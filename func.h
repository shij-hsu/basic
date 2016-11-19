extern int yylineno;

typedef struct yy_buffer_state *YY_BUFFER_STATE;
//#define YY_BUF_SIZE 65536
#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE 65536
#endif
YY_BUFFER_STATE yy_create_buffer(FILE*,int);
void yypush_buffer_state(YY_BUFFER_STATE);
void yypop_buffer_state();
void yyerror(const char *s,...);
int yylex(void);
extern FILE* yyin;

#define MAX_BETA_STEPS 32

struct symbol{
    char *name;         //名字
    struct term *t;     //term
    char type;          //是否处于顶层 T - top, B - bind, N - unknow
};

typedef enum _TYPE{
    APP=256,ABS,VAR,    //APP - application
                        //ABS - abstract
                        //VAR - varibale
}TYPE;

struct term{
    TYPE nodetype;
    struct symbol *sym;
    struct term *l;
    struct term *r;
};

#define NHASH 9997
struct symbol symtab[NHASH];
struct symbol *lookup(char *);

struct term *new_var_term(struct symbol*s);
struct term *new_abs_term(struct symbol *s, struct term *t);
struct term *new_app_term(struct term *l, struct term *r);
struct term *copy_term(struct term *origin);
void delete_term(struct term* t);
int compare_term(struct term *t1,struct term *t2);
struct term* subst(struct term *e1, struct symbol *v, struct term *e2);

struct term *beta(struct term *t);
struct term *beta_(struct term *t);
struct term *beta_s(struct term *t);
struct term* alpha(struct term* e);
int dir();
int help();


int free_in_term(struct term* e,struct symbol *v);
int print_term(struct term *t);
