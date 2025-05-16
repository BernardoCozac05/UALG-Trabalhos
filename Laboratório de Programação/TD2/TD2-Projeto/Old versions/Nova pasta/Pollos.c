#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>

/* ===================  dynvec  =================== */
typedef struct {
    void  *data;
    size_t length;
    size_t cap;
    size_t elem_size;
} dynvec;

static dynvec *dynvec_create(size_t elem_size) {
    dynvec *v = malloc(sizeof *v);
    if (!v) { perror("malloc dynvec"); exit(EXIT_FAILURE); }
    v->elem_size = elem_size;
    v->length = 0;
    v->cap = 16;
    v->data = malloc(v->cap * elem_size);
    if (!v->data) { perror("malloc data"); exit(EXIT_FAILURE); }
    return v;
}

static void dynvec_free(dynvec *v, void (*elem_free)(void*)) {
    if (!v) return;
    if (elem_free)
        for (size_t i = 0; i < v->length; ++i)
            elem_free((char*)v->data + i*v->elem_size);
    free(v->data);
    free(v);
}

static void dynvec_reserve(dynvec *v,size_t need){
    if(need<=v->cap) return;
    while(v->cap<need) v->cap*=2;
    v->data=realloc(v->data,v->cap*v->elem_size);
    if(!v->data){perror("realloc dynvec");exit(EXIT_FAILURE);}
}

static void *dynvec_push(dynvec *v,const void*elem){
    dynvec_reserve(v,v->length+1);
    void *dst=(char*)v->data+v->length*v->elem_size;
    memcpy(dst,elem,v->elem_size);
    v->length++;return dst;
}

static size_t dynvec_index(dynvec *v, void *elem, int (*cmp)(const void*,const void*)) {
    for(size_t i=0;i<v->length;++i)
        if(cmp((char*)v->data+i*v->elem_size,elem)==0)
            return i;
    errno=ENOENT;
    return v->length;
}

static bool dynvec_contains(dynvec *v, void *elem, int(*cmp)(const void*,const void*)) {
    return dynvec_index(v,elem,cmp)!=v->length;
}

static void dynvec_sort(dynvec *v, int (*cmp)(const void *, const void *)) {
    qsort(v->data, v->length, v->elem_size, cmp);
}

/* ============= Estrutura das palavras ============== */
typedef struct { char *word; size_t count; } Word;

static int cmp_alpha(const void *a,const void *b){
    return strcmp(((const Word*)a)->word, ((const Word*)b)->word);
}

static int cmp_freq(const void *a,const void *b){
    const Word *wa=a,*wb=b;
    if(wa->count!=wb->count) return (wa->count<wb->count)?1:-1;
    size_t la=strlen(wa->word), lb=strlen(wb->word);
    if(la!=lb) return (la<lb)?1:-1;
    return strcmp(wa->word, wb->word);
}

static void free_word(void *p){ Word *w=p; free(w->word); }

/* ===================== util ===================== */
static char *dup_token(const char *src,size_t len){
    char *s=malloc(len+1); if(!s){perror("malloc tok");exit(EXIT_FAILURE);} 
    memcpy(s,src,len); s[len]='\0'; return s; 
}

static void insert_or_inc(dynvec *v,char *tok){
    Word key={tok,0};
    if(dynvec_contains(v,&key,cmp_alpha)){
        size_t idx=dynvec_index(v,&key,cmp_alpha);
        Word *w=(Word*)((char*)v->data+idx*sizeof(Word));
        w->count++; free(tok);
        return;
    }
    size_t pos=0; while(pos<v->length && cmp_alpha(&key,(char*)v->data+pos*sizeof(Word))>0) pos++;
    dynvec_reserve(v,v->length+1);
    memmove((char*)v->data+(pos+1)*sizeof(Word),
            (char*)v->data+pos*sizeof(Word),
            (v->length-pos)*sizeof(Word));
    Word nw={tok,1}; memcpy((char*)v->data+pos*sizeof(Word),&nw,sizeof nw);
    v->length++;
}

/* ===================== main ===================== */
int main(void){
    size_t N;
    if(scanf("%zu", &N) != 1){
        fprintf(stderr, "Need N\n");
        return 1;
    }

    int ch;
    char buf[256];
    size_t len = 0;
    dynvec *dict = dynvec_create(sizeof(Word));

    while((ch = getchar()) != EOF){
        if(isalpha((unsigned char)ch)){
            if(len < sizeof buf - 1) buf[len++] = ch;
        } else if(len){
            insert_or_inc(dict, dup_token(buf, len));
            len = 0;
        }
    }
    if(len) insert_or_inc(dict, dup_token(buf, len));

    dynvec_sort(dict, cmp_freq);

    size_t out = (N < dict->length) ? N : dict->length;
    Word *arr = dict->data;
    for(size_t i = 0; i < out; ++i)
        printf("%zu %s\n", arr[i].count, arr[i].word);

    dynvec_free(dict, free_word);
    return 0;
}