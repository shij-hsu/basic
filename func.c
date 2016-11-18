#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "func.h"

static unsigned symhash(char *sym){
    unsigned int hash=0;
    unsigned c;
    while (c=*sym++)hash=hash*9^c;
    return hash;
}

struct symbol *lookup(char *sym){
    struct symbol *sp=&symtab[symhash(sym)%NHASH];
    int scount=NHASH;
    while(--scount>=0){
        if(sp->name && !strcmp(sp->name,sym)){return sp;}
        if(!sp->name){
            sp->name=strdup(sym);
            sp->t=NULL;
            sp->type='N';
            return sp;
        }
        if(++sp>=symtab+NHASH)sp=symtab;
    }
    yyerror("symbol table overflow\n");
    abort();
}

struct term *new_var_term(struct symbol*s){
    struct term *ret=(struct term*)malloc(sizeof(struct term));
    if(!ret)return NULL;
    ret->nodetype=VAR;
    ret->sym=s;
    ret->l=NULL;
    ret->r=NULL;
    return ret;
}
struct term *new_abs_term(struct symbol *s, struct term *t){
    struct term* ret=(struct term*)malloc(sizeof(struct term));
    if(!ret)return NULL;
    ret->nodetype=ABS;
    ret->sym=s;
    ret->l=t;
    ret->r=NULL;
    return ret;
}
struct term *new_app_term(struct term *l, struct term *r){
    struct term* ret=(struct term*)malloc(sizeof(struct term));
    if(!ret)return NULL;
    ret->nodetype=APP;
    ret->sym=NULL;
    ret->l=l;
    ret->r=r;
    return ret;
}
struct term *copy_term(struct term *origin){
    if(!origin)return NULL;
    struct term *new_term=(struct term *)malloc(sizeof(struct term));
    if(!new_term)return NULL;
    new_term->nodetype=origin->nodetype;
    new_term->sym=origin->sym;
    new_term->l=copy_term(origin->l);
    new_term->r=copy_term(origin->r);
    return new_term;
}
int compare_term(struct term *t1,struct term *t2){
    if(t1==NULL){
        if(t2==NULL)return 1;
        else return 0;
    }else if(t2==NULL)return 0;
    else if(t1->nodetype!=t2->nodetype)return 0;
    switch (t1->nodetype)
    {
        case VAR:return (!strcmp(t1->sym->name,t2->sym->name));
        case ABS:return (!strcmp(t1->sym->name,t2->sym->name) &&
                        compare_term(t1->l,t2->l));
        case APP:return (compare_term(t1->l,t2->l) &&
                        compare_term(t1->r,t2->r));
    default:
        break;
    }
}

//化简lambda term
/* e1 { v/e2 } =
    e1.type=var => if e1=v then e2 else e1
    e1.type=app => e1 = e11 e22 => e11 {v/e2} e22 {v/e2}
    e1.type=bas => e1 = \x.e11 => if v=x then e1 else \x.(e11 {v/e2})
*/
struct term *subst(struct term *e1, struct symbol *v, struct term *e2){
    char *v_name=v->name;
    if(e1->nodetype==VAR){
        if(!strcmp(v_name,e1->sym->name))
            return copy_term(e2);
        else return copy_term(e1);
    }
    else if(e1->nodetype==APP){
        return new_app_term(subst(e1->l,v,e2),subst(e1->r,v,e2));
    }
    else /*if(e1->nodetype==ABS)*/{
        if(!strcmp(v_name,e1->sym->name))
            return copy_term(e1);
        else return new_abs_term(e1->sym,subst(e1->l,v,e2));
    }
}

/* (fun v=>e1) e2=(beta)=> e1 {v/e2} */
struct term *beta(struct term *t){
    if(!t)return NULL;
    if(t->nodetype!=APP)return copy_term(t);
    else{
        struct term* fun=t->l;
        if(!fun){
            return copy_term(t);
        }
        if(fun->nodetype!=ABS){
            return copy_term(t);
        }
        else{
            return subst(fun->l,fun->sym,t->r);
        }
    }
}
struct term *beta_s(struct term *t){
    if(!t)return NULL;
    struct term *rep=NULL;
    switch (t->nodetype)
    {
        case VAR:rep = copy_term(t); break;
        case ABS:rep = new_abs_term(t->sym,beta_s(t->l));break;
        case APP:
            rep = beta(t);
            if(compare_term(rep,t)!=1)rep = beta_s(rep);
            else rep = new_app_term(beta_s(rep->l),beta_s(rep->r));
            break;
    default:
        break;
    }
    if(rep->nodetype==APP){
        t = beta(rep);
        if(compare_term(t,rep)==1)rep=t;
        else rep=beta_s(t);
    }
    return rep;
}


int print_term(struct term *t){
    if(!t)printf("[INVALID]");
    switch (t->nodetype)
    {
        case VAR:printf("%s",t->sym->name);break;
        case ABS:
            printf("\\%s.",t->sym->name);
            if(t->l)print_term(t->l);
            else printf("[INVALID]");
            break;
        case APP:
            if(!t->l) printf("[INVALID]");
            else if(t->l->nodetype!=VAR){
                printf("(");
                print_term(t->l);
                printf(")");
            }else print_term(t->l);
            printf(" ");
            if(!t->r)printf("[INVALID]");
            else if(t->r->nodetype!=VAR){
                printf("(");
                print_term(t->r);
                printf(")");
            }else print_term(t->r);
            break;
    default:
        break;
    }
}
