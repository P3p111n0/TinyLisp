# TinyLISP
> je doopravdy tiny

# Kompilace
``` 
make
```
případně
```
make debug
```
pokud chcete zaplé sanitizery.

# Spuštění
```
./TinyLisp <filename>
```
V examples mají všechny soubory příponu `.tl`, ale nezáleží na ní. 

# Podporované featury
- rekurze u pojmenovaných funkcí
- vyhodnocování aritmetiky v postfixové notaci
- ify 
- lambda funkce 
- volání funkcí 

# TODO 
- Parser není moc dobrý, ale lépe to neumím :( Po PJP ho předělám. 
- Nelze porovnávat jiné typy než inty
- Nelze používat unární minus 
- Není implementovaný quoting, let, letrec a builtiny type `null?` a podobné 
