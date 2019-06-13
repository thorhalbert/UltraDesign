list quick ucadsrc/#?.c >a.tmp
list quick ucaddefs/#?.h >b.tmp
list quick ucadsrc/dialog/#?.yacc >c.tmp
list quick ucadsrc/dialog/#?.lex >d.tmp
list quick pusrc/#?.c >e.tmp
list quick pudefs/#?.h >f.tmp
list quick pdrsrc/#?.h >g.tmp
list quick pdrsrc/#?.c >h.tmp
list quick pdrsrc/#?.asm >i.tmp
list quick converters/#?.c >j.tmp
list quick converters/#?.h >k.tmp
join a.tmp b.tmp c.tmp d.tmp e.tmp f.tmp g.tmp h.tmp i.tmp j.tmp k.tmp as tar.tmp
