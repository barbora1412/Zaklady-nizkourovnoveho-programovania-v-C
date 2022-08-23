#include <assert.h>
#include <stdlib.h>
#include <string.h>

//***********************************************
//              IMPORTANT NOTICE                *
//***********************************************
//  If you need to add more headers, which      *
//  you CAN, add them AFTER THIS LINE           *

//  but BEFORE the following one:               *
#include "memoryVM.h"
//  otherwise your code MAY FAIL TO COMPILE!    *
//                                              *
//  You have been warned.                       *
//***********************************************


struct node2D
{
    int count;
    void *data;
};

/**
 * \brief Doubly linked list node.
 */
struct node
{
    int ref_count;
    union
    {
        void *data; /**< pointer to node data */
        void **data2d; /** pointer to main array2D */
    } data;
    struct node2D *data2d;
    int size2d;
    struct node *prev; /**< previous node        */
    struct node *next; /**< next node            */
};

/**
 * \brief Doubly linked list.
 */
struct list
{
    struct node *head; /**< list's first node */
    struct node *tail; /**< list's last node  */
};

static struct list memory_register; /** <active only inside my file */

void addNode(struct node *node)
{
    // the node will be added to the rear
    node->prev = memory_register.tail;
    node->next = NULL;

    // if the list is empty, set the head to the new node
    if (memory_register.tail == NULL)
        memory_register.head = node;
    // otherwise tell the tail that it has another sibling
    else
        memory_register.tail->next = node;

    // we have a new tail
    memory_register.tail = node;
}

void delNode(struct node *node)
{
    if (node == NULL)
        return;

    // entangle list nodes
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;

    // recover head and tail if necessary
    if (memory_register.head == node)
        memory_register.head = node->next;
    if (memory_register.tail == node)
        memory_register.tail = node->prev;

    // the node is useless now
    free(node);
}

bool convert_from2D(void *data, int count)
    //converts residual subarray to an ordinary node
{
    struct node *node = NULL;

    node = malloc(sizeof(struct node));
    if (node == NULL)
        return false;
    memset(node, 0, sizeof(struct node));
    node->ref_count = count;
    node->data.data = data;
    node->data2d = NULL;
    node->size2d = 0;
    addNode(node);
    return true;
}

bool initVM()
{

    memory_register.head = NULL;
    memory_register.tail = NULL;
    return true;
}

void clearVM(void)
{
    struct node *node, *pnode;
    node = memory_register.head;

    while (node != NULL) {
        if (node->data2d != NULL) {
            // if 2D array - iterate through each node and free
            for (int i = 0; i < node->size2d; i++) {
                if (node->data.data2d[i] != NULL)
                    free(node->data.data2d[i]);
            }
            free(node->data2d);
        }
        free(node->data.data);

        //can't delete node because we need node.next
        pnode = node;
        node = node->next;
        delNode(pnode);
    }
}

void *allocate(size_t size)
{
    struct node *node = NULL;

    //we can't allocate 0
    if (size == 0)
        return NULL;
    node = malloc(sizeof(struct node));
    if (node == NULL)
        return NULL;
    memset(node, 0, sizeof(struct node));
    node->ref_count = 1;
    node->data.data = malloc(size);
    if (node->data.data == NULL) {
        free(node);
        return NULL;
    }
    memset((void *)node->data.data, 0, size);
    node->data2d = NULL;
    node->size2d = 0;
    addNode(node);

    // return adress of data after allocation
    return node->data.data;
}

void acquire(void *memory)
{
    struct node *node = NULL;
    int i;

    if(memory == NULL)
        return;
    assert(memory != NULL);
    node = memory_register.head;
    while (node != NULL) {
        if (node->data.data == memory) {
            node->ref_count++;
            node = memory_register.tail;
        } else if (node->data2d != NULL) {
            for (i = 0; i < node->size2d; i++) {
                if (node->data2d[i].data == memory) {
                    node->data2d[i].count++;
                    node = memory_register.tail;
                    break;
                }
            }
        }
        node = node->next;
    }
}

bool release(void *memory)
{
    struct node *node = NULL;
    int i;

    if(memory == NULL)
        return false;
    node = memory_register.head;
    while (node != NULL) {
        if (node->data.data == memory) {
            if (node->ref_count > 1) {
                node->ref_count--;
                node = memory_register.tail; //set last node for quit from cycle
            } else {

                if (node->data2d != NULL) {
                    // if main Array release,
                    // i am converting nodes individually so they wouldn't get lost
                    for (i = 0; i < node->size2d; i++) {
                        // if exist subarray, convert to ordinary node
                        if (node->data.data2d[i] != NULL)
                            assert(convert_from2D(node->data2d[i].data, node->data2d[i].count));
                    }
                    free(node->data2d);
                }
                free(node->data.data);
                delNode(node);
                return true;
            }

          // check if it is subarray's memory
        } else if (node->data2d != NULL) {
            for (i = 0; i < node->size2d; i++) {
                if (node->data2d[i].data == memory) {
                    if (node->data2d[i].count > 1) {
                        node->data2d[i].count--;
                        node = memory_register.tail; // set last node
                        break;
                    }
                    if (node->data2d[i].count == 1) {
                        free(node->data.data2d[i]);
                        //during decremention node could be changed to NULL
                        node->data.data2d[i] = NULL;
                        node->data2d[i].data = NULL;
                        node->data2d[i].count = 0;
                        return true;
                    }
                }
            }
        }
        node = node->next;
    }
    return false;
}

void *allocateArray(size_t elementSize, size_t elementCount)

{
    size_t size = 0;
    size = elementSize * elementCount;

    return allocate(size);
}

void **allocateArray2D(size_t elementSize, size_t subarrays, size_t *elementCounts)
{
    struct node *node; /** nodes adress */
    void *psize;       /** additional variable for allocation */
    size_t size = 0;   /** size for memory allocation */
    struct array
    { /** structure of main array that has array of pointers to subarrays */
        void *subarrays;
    };
    unsigned int i,j;

    if (elementSize == 0 || subarrays == 0) {
        return NULL;
    }
    node = malloc(sizeof(struct node));
    if (node == NULL)
        return NULL;
    memset(node, 0, sizeof(struct node));
    size = sizeof(struct node2D) * subarrays; /** subarrays counters and references */
    node->data2d = malloc(size);
    if (node->data2d == NULL) {
        free(node);
        return NULL;
    }
    memset(node->data2d, 0, size);

    size = sizeof(size_t *) * subarrays; /** main array */
    node->data.data2d = malloc(size);
    if (node->data.data2d == NULL) {
        free(node->data2d);
        free(node);
        return NULL;
    }
    memset(node->data.data2d, 0, size);

    for (i = 0; i < subarrays; i++) { /** iterating through subarrays */
        if (elementCounts == NULL) {
            size = elementSize * subarrays; /** if elementCounts is Null then subarrays has same sizes */
        } else {
            size = elementSize * elementCounts[i]; /** elementCounts is not Null then sizes are in elementCounts */
        }
        if (size > 0) {
            psize = malloc(size); /** allocated memory for subarray */
            if (psize == NULL) {  /** if there is not enough memory */
                if (i > 0)
                    for (j = i - 1; j > 0; j--) { /** free all created subarrays */
                        if(node->data.data2d[j] != NULL)
                            free(node->data.data2d[j]);
                    }
                free(node->data2d);      /** free additional array */
                free(node->data.data2d); /** free main array */
                free(node);
                return NULL;
            }
            memset(psize, 0, size);       /** set subarray to 0s */
            node->data.data2d[i] = psize; /** adress of subarray to main array */
            node->data2d[i].data = psize; /** adress of subarray to array of counter references */
            node->data2d[i].count = 1;    /** reference of subarray is 1 */
        } else {
            node->data.data2d[i] = NULL;  /** adress of subarray to main array */
            node->data2d[i].data = NULL;  /** adress of subarray to array of counter references */
            node->data2d[i].count = 0;    /** reference of subarray is 1 */

        }
    }

    node->size2d = subarrays;
    node->ref_count = 1;
    addNode(node);
    return node->data.data2d;
}

void acquireArray2D(void **array)
{
    struct node *node = NULL;
    int i;

    if (array == NULL)
        return;
    node = memory_register.head;
    while (node != NULL) {
        if (node->data2d != NULL && node->data.data2d == array) {
            node->ref_count++;
            for (i = 0; i < node->size2d; i++) {
                node->data2d[i].count++;
            }
            break; /** found pointer to main array */
        }
        node = node->next;
    }
}

bool releaseArray2D(void **array)
{
    struct node *node = NULL;
    int i;

    if(array == NULL)
        return false;
    node = memory_register.head;
    while (node != NULL) {
        if (node->data2d != NULL && node->data.data2d == array) {
            for (i = 0; i < node->size2d; i++) {
                if (node->data2d[i].data == NULL)
                    continue;
                if (node->data2d[i].count > 1) {
                    node->data2d[i].count--;
                } else {
                    free(node->data.data2d[i]);
                    node->data.data2d[i] = NULL;
                    node->data2d[i].data = NULL;
                    node->data2d[i].count = 0;
                }
            }
            if (node->ref_count > 1) /** check main array */
                node->ref_count--;
            else {
                for (int i = 0; i < node->size2d; i++) {

                    //if subarrays have references >1 i have to keep them
                    if (node->data2d[i].data != NULL)
                        assert(convert_from2D(node->data2d[i].data, node->data2d[i].count));
                }
                free(node->data.data2d);
                free(node->data2d);
                delNode(node);
                return true;
            }
        }
        node = node->next;
    }
    return false;
}
