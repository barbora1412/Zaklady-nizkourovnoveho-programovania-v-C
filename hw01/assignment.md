---
title: "HW01: Kontrola pravopisu"
layout: "homework"
list-of-files: ["hw01.c"]
publish: now
deadline-early: "2018-03-10 24:00"
deadline-final: "2018-03-13 24:00"
publish-solution: ~
author: "Jan Tušil"
author-email: jan.tusil@mail.muni.cz
---

Doplnění zadání:

**2018-02-02 12:49**
: Definice věty zpřesněna a zjednodušena.

**2018-02-02 16:32**
: Popis `allSentencesAreClosed` zpřesněn.

**2018-02-02 20:53**
: Přidán odkaz na vzorové řešení a jeho použití.

**2018-03-01 13:55**
: Nová definice pro znaky začínající větu.

**2018-03-02 17:20**
: Oslabeny požadavky na funkci `noParenthesesSentenceCrossing` tak,
  aby byla snadno implementovatelná (a aby referenční implementace
  těmto požadavkům vyhovovala).

**2018-03-02 17:50**
: Doplněna informace o znaku uvozovek.

**2018-03-04 15:30**
: Oslabeny požadavky na funkci `spaceAfterSeparators` tak,
  aby zadání nebylo v rozporu s běžnými pravidly pravopisu.

**2018-03-07 20:00**
: Bonus: jak použít nevyužitý parametr funkce.

## Představení úkolu

V této domácí úloze budeme vytvářet jednoduchý nástroj na kontrolu pravopisu. 
Tento je pro jednoduchost řešen sadou funkcí, kde každá implementuje nějaké
pravidlo. Do vínku dostanete i podpůrný kód, kterého si ale nemusíte všímat -
slouží jen pro snazší testování.

---

## Zadání

1. Každé z níže uvedených pravidel implementujte jako samostatnou funkci, uvnitř souboru ``hw01.c``.
2. Jako ukázku toho, co po vás chceme, máte k dispozici jedno implementované pravidlo. 
Nejspíše ale obsahuje chyby a bude jej tedy potřeba zkontrolovat a opravit.

Abyste si mohli svůj kód otestovat, přibalili jsme vám i soubor ``hw01_main.c``,
ze kterého vznikne spustitelný program ``hw01_main``. Ten se vás po spuštění dotáže
na řetězec, který následně prověří vůči všem pravidlům.

Jako poslední máte k dispozici soubor ``hw01_tests.c``, do kterého si můžete doplnit
vlastní testy. Na řadě míst ale bude kód pokročilejší než to co v tuto chvíli píšete vy,
nezoufejte tedy pokud něčemu neporozumíte. Pamatujte, že jediným odevzdávaným souborem
je ``hw01.c``, ve zbývajících souborech si tedy (s přiměřenou opatrností) můžete dělat
co chcete.

Pro úplnost okomentujeme i zbývající soubory -- s příponou ``.h``. Tyto slouží
k propojení jednotlivých souborů s příponou ``.c`` podobně, jako hlavičky standardní
knihovny umožňují propojovat váš kód s operačním systémem.

---

### Základní pojmy

*V úloze pracujeme s následující skupinou znaků: velká a malá písmena (`'A'`-`'Z'`, `'a'`-`'z'`),
číslice (`'0'`-`'9'`), závorkové znaky (`'('`, `')'`, `'['`, `']'`, `'{'`, `'}'`),
oddělovače (`'.'`, `','`, `'?'`, `'!'`), znak uvozovek (`'"'`),
a mezera (`' '`). Znak, který není mezerou, nazýváme nemezerový.*{: class="text-primary" }

Řetězec je rozdělen na věty. Věta začíná ~~alfanumerickým (česky: písmena a čísla)~~{: class="text-muted"}
*nemezerovým nezávorkovým*{: class="text-primary" } znakem 
a končí tečkou, vykřičníkem, otazníkem nebo koncem řetězce. Přesněji,
věta je souvislou posloupností ~~(nejen alfanumerických)~~{: class="text-muted" } znaků v řetězci
s následujícími vlastnostmi:

1. *Pojem "začínač věty" označuje znak, který není ani mezerou, ani závorkovým znakem.*{: class="text-primary"}
2. Za první znak první věty považujeme první ~~alfanumerický znak~~{: class="text-muted" }
   ~~nemezerový znak~~{: class="text-muted" } *začínač věty*{: class="text-primary"} v řetězci.
3. Pojem "ukončovač věty" označuje tečku (`'.'`), vykřičník (`'!'`) a otazník (`'?'`).
4. Výskyt ukončovače věty je vždy posledním znakem věty.
5. Prvním výskytem ~~alfanumerického znaku~~{: class="text-muted" }
   ~~alfanumerického znaku~~{: class="text-muted" }
   *začínače věty*{: class="text-primary"}
   po ukončovači věty začíná nová věta
   ~~(mezery považujeme za tzv. bílé znaky, nikoliv alfanumerické)~~{: class="text-muted" }.
6. Konec řetězce ukončuje větu.

---

## Požadavky

Implementujte pravidla v souboru `hw01.c`, každé do vlastní funkce:

1. `noTwoConsecutiveSpaces`  
   Řetězec nesmí obsahovat dvě po sobě jdoucí mezery (znak `' '`).
   Tato funkce je již implementována, nicméně implementace obsahuje chybu.

2. `sentenceStartsWithUppercase`  
   Každá věta musí začít velkým písmenem.

3. `allSentencesAreClosed`  
   Každá věta (zejména tedy i poslední) musí být ukončena ukončovačem věty.
    *Přesněji, poslední znak každé věty musí být jedním z ukončovačů věty.*{: class="text-primary"}

4. `noSpaceBeforeSeparators`  
   Před tečkou, čárkou, vykřičníkem či otazníkem se nesmí nacházet mezera.

5. `spaceAfterSeparators`  
   Pokračuje-li za tečkou, čárkou, vykřičníkem či otazníkem text, musí pokračování
   předcházet mezera. *Návratová hodnota této funkce je definována jen v případě,
   že vstupní řetězec neobsahuje závorky. Jinými slovy, tuto funkci
   budeme testovat jenom na vstupech neobsahujících závorky.*{: class="text-primary"}

6. `properParenthesesPairing`  
   V textu se mohou vyskytovat tyto druhy závorek `()`, `{}`, `[]`.
   Každá otevírací závorka musí být uzavřena odpovídající závorkou stejného typu.
   Každé uzavírací závorce předchází "její" otevírající závorka.
   Závorky nesmí být vnořené. Což mimochodem zakazuje jak situace tvaru `({})`, tak `({)}`.

---

## Bonusové rozšíření

Tato pravidla jsou bonusová, není tedy nutné je vypracovat.
Za jejich vypracování je však možné získat tři body navíc, nad rámec
standardních devíti bodů.

1. `noParenthesesSentenceCrossing`  
   Závorky nesmí křížit větnou strukturu textu. Je-li nějaká závorka otevřena
   před začátkem věty, nesmí být uzavřena uvnitř věty, ale jen mezi větami
   (za koncem nějaké věty, před začátkem věty následující).
   Naopak, je-li nějaká závorka otevřena uvnitř věty, musí být uzavřena
   ještě uvnitř téže věty. *Návratová hodnota této funkce je definována jen v případě, že by
   funkce `properParenthesesPairing` na stejném vstupu prošla (vrátila `true`),
   tedy pokud je vstup správně uzávorkovaný a neobsahuje vnořené závorky.
   V opačné případě může tato funkce vrátit libovolnou hodnotu,
   měla by však něco vrátit (zejména by neměla cyklit nebo způsobit pád).
   Jinými slovy: tuto funkci budeme testovat jen na dobře uzávorkovaných
   vstupech neobsahující vnořené závorky.*{: class="text-primary"}

2. `noSpaceBeforeClosingOrAfterOpeningApostrophe`  
   Text může obsahovat dvojité uvozovky (znak `"`), ale vždy jen v páru.
   Navíc otevírací uvozovka nesmí být následována mezerou,
   a podobně uzavírací uvozovka nesmí být předcházena mezerou.

*Tyto funkce tedy implementovat nemusíte, v jazyce C však musí být každý parametr funkce
někde použitý. Triviální implementace bonusových funkcí*{: class="text-primary"}

```c
bool noSpaceBeforeClosingOrAfterOpeningApostrophe(char str[], size_t length) {
    return true;
}
```
*pak způsobí, že překladač vypíše varování:*{: class="text-primary"}
```
In function ‘noSpaceBeforeClosingOrAfterOpeningApostrophe’:
warning: unused parameter ‘str’
```

*což při použití `-Werror` znamená chybu překladu. Řešením je parametr
použít tím způsobem, že ho převedeme na typ `void`*{: class="text-primary"}:
```c
bool noSpaceBeforeClosingOrAfterOpeningApostrophe(char str[], size_t length) {
    (void) str;
    (void) length;
    return true;
}
```


---

## Testy

Soubor ``hw01_tests.c`` obnáší malý základ testů jednotlivých pravidel.
Větší část z těchto testů jsou pouze kostry, které ve skutečnosti nic
netestují, avšak umožní vám snadno zavést testy jednotlivých pravidel. 
Své testy doplňujte místo jednotlivých todo v souboru.

Je naprosto v pořádku (ba dokonce žádoucí) testovat každou funkci více
testovacími případy. Začněte jednoduchými, až triviálními případy, které
postupně komplikujte. Je také vhodné testovat nejen příklad, ale i protipříklad.
Kupříkladu test funkce ``allSentencesAreClosed`` obsahující pouze

```c
SHOULD_PASS("Veta cislo jedna.");
```

nikdy neodhalí (programátorovi zjevnou) chybu ve funkci

```c
bool allSentencesAreClosed(char unused[], size_t length) {
    return true;
}
```

Příklad o něco lepšího testu:

```c
SHOULD_PASS("Spravne ukoncena veta.");
SHOULD_FAIL("Spatne ukoncena veta");
SHOULD_PASS("Vykricnik na konci je taky ukoncovac!");
SHOULD_PASS("Opravdu?");
SHOULD_PASS("Tahle veta ma taky projit, ze?");
SHOULD_PASS("I tohle je testovaci pripad. Protoze ma vice vet! Ale musi byt zakonceny ruznymi oddelovaci?");
SHOULD_FAIL("Nezapomente. Jedna tecka nestaci");
SHOULD_PASS("Libovolny pocet mezer      nezpusobi pad testu - jine pravidlo.");
```

Takových testů by šla napsat spousta. Ale zbytek už necháme na vás. Hodně štěstí.

---

## Vzorové řešení

Vzorové řešení je k dispozici na Aise ~~a linuxových počítačích~~{: class="text-muted"} v `/home/kontr/hw01/spellcheck`.
Příklad použití:

```sh
[xtusil@aisa ~]$ echo -n "Ahoj. " | /home/kontr/hw01/spellcheck
correct   sentenceStartsWithUppercase
correct   noSpaceBeforeSeparators
correct   noTwoConsecutiveSpaces
correct   spaceAfterSeparators
correct   properParenthesesPairing
correct   noParenthesesSentenceCrossing
correct   allSentencesAreClosed
correct   noSpaceBeforeClosingOrAfterOpeningApostrophe
```

## Poznámky

Každá z funkcí by měla ověřovat jen a pouze své pravidlo.
Pro zjednodušení implementace můžete použít pomocnou funkci

```c
bool isOneOf(char c, char const str[])
```

která vrátí `true` právě tehdy, když se znak `c` vyskytuje v řetězci `str`.
Do kódu této funkce prosím nezasahujte.

Pokud budete chtít implementovat nějaké své pomocné funkce, učiňte tak prosím
uvnitř ``hw01.c``. Ač si do svého repozitáře můžete přidat i dodatečné soubory
(a je vhodné tak učinit),
Kontr bude brát v úvahu pouze ``hw01.c``. P.S. Není dobrý nápad dávat do gitu
výsledné binární soubory, jako například ``hw01_main``.


