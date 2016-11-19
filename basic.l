%option yywrap yylineno 
%{
    #include "func.h"
    #include "basic.tab.h"
%}

%%
"let"                   { return LET; }
"print"                 { return PRINT; }
"load"                  { return LOAD; }
"beta"                  { return BETA; }
"beta*"                 { return BETA_; }
"alpha"                 { return ALPHA; }
"dir"                   { return DIR; }
"exit"                  { return EXIT; }
"help"                  { return HELP; }


[a-zA-Z_][a-zA-Z_0-9]*  { yylval.s = lookup(yytext); return ID; }
[0-9]+                  { yylval.i = atoi(yytext); return NUM; }
\. |
"(" |
")" |
"\\" |
"="                     { return yytext[0]; }
\n                      { return EOL; }
<<EOF>>                 { return EOF_; }
[ >\r\t] {}
"//".*      {}
.                       { printf("#%d Mystery character %c\n",yylineno, yytext[0]); }

%%
