%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <math.h>
    #include "func.h"
%}

%union{
    int i;
    struct symbol *s;

    struct term *t;
};

%token <i> NUM
%token <s> ID

%token LET IN PRINT LOAD
    ALPHA BETA BETA_
    IOTA DELTA EOL EOF_

%type <t> term term_list

%start calc_list

%%
stmt: LET ID '=' term_list
        { $2->t=$4; $2->type='T'; }
    | PRINT ID 
        {
            if($2->type=='T'){
                printf("%s = ",$2->name);
                print_term($2->t); 
            }
            else printf("Unknown symbol");
            printf("\n"); 
        }
    | LOAD ID
        { 
            char file_name[256]={0};
            sprintf(file_name,"%s.k",$2->name);
            FILE *file=fopen(file_name,"rw+");
            if(!file){
                printf("Failed, moudles loaded: none.\n");
            }
            else
            {
                YY_BUFFER_STATE bp =
                    yy_create_buffer(file,YY_BUF_SIZE);
                yypush_buffer_state(bp);
                printf("OK, moudles loaded: %s.\n",$2->name);
            }
        }
    | term_list
        {
            print_term($1);
            printf("\n");
        }
    ;

term_list:term 
        { $$ = $1; }
    | term_list term 
        { $$ = new_app_term($1, $2); }
    ;

term: ID 
        {
            if($1->t) $$ = $1->t;
            else $$ = new_var_term($1); 
        }
    | '\\' ID '.' term_list
        { $$ = new_abs_term($2, $4); }
    | '(' term_list')' 
        { $$ = $2; }
    | BETA_ term_list
        {
            $$ = beta_s($2);
        }
    | BETA term_list
        {
            $$ = beta($2);
        }
    ;

calc_list:
    | calc_list EOL {
        if(yyin==stdin)printf("> ");
    }
    | calc_list stmt EOL {
        if(yyin==stdin)printf("> ");
    }
    | calc_list error EOL {
        yyerrok; if(yyin==stdin)printf("> "); 
    }
    | calc_list EOF_ {
        yypop_buffer_state();
    }
    ;
%%

void yyerror(const char *s,...)
{
    va_list ap;
    va_start(ap,s);
    fprintf(stderr,"#%d: error: ",yylineno);
    vfprintf(stderr,s,ap);
    fprintf(stderr,"\n");
}

int main(){
    printf("> ");
    return yyparse();
}