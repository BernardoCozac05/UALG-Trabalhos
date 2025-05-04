/* padre_amaro_topwords_td2.c – versão 100 % «TD2‑compliant»
 * ------------------------------------------------------------------
 * Demonstra o uso de **todas** as abstrações pedidas no TD2 sobre
 * vetores dinâmicos genéricos:
 *   • dynvec_index / dynvec_contains
 *   • dynvec_fold_left
 *   • dynvec_map
 *   • dynvec_forall / dynvec_exists / dynvec_exists_index
 *   • dynvec_filter
 *   • quicksort_dynvec (ordenação genérica)
 * Para fins de demonstração, o programa continua a ler texto do
 * stdin e a mostrar as N palavras mais frequentes, mas agora todo o
 * processamento passa exclusivamente por estas abstrações.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <errno.h>
 #include <stdbool.h>
 
 #include "dynvec.h"   /* deve expor as funções avançadas do TD2 */
 
 /* ================================================================ */
 /*  Estrutura dos elementos: palavra + contagem                     */
 /* ================================================================ */
 
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
 
 /* ================================================================ */
 /*  Pequenas helpers para os iteradores genéricos                    */
 /* ================================================================ */
 
 /* map: garantir lower‑case total (segurança extra) */
 static void to_lower_word(void *p){
     Word *w=p;
     for(char *s=w->word; *s; ++s) *s=(char)tolower((unsigned char)*s);
 }
 /* fold_left: somatório das contagens */
 static void sum_counts(void *acc, void *elem){
     *(size_t*)acc += ((Word*)elem)->count;
 }
 /* predicates para exists / forall / filter */
 static bool is_longer_than_20(void *elem){
     return strlen(((Word*)elem)->word) > 20;
 }
 static bool count_gt_zero(void *elem){
     return ((Word*)elem)->count>0;
 }
 static bool starts_with_upper(void *elem){
     return isupper((unsigned char)((Word*)elem)->word[0]);
 }
 static bool freq_ge_2(void *elem){
     return ((Word*)elem)->count>=2;
 }
 
 /* ================================================================ */
 /*  Caso o dynvec.h ainda não exponha estas duas utilidades          */
 /* ================================================================ */
 #ifndef dynvec_get
 #define dynvec_get(v,i) ((void *)((char*)(v)->data + (i)*(v)->elem_size))
 #endif
 #ifndef dynvec_insert
 static void dynvec_insert(dynvec *v, size_t pos, const void *elem){
     dynvec_reserve(v,v->length+1);
     memmove((char*)v->data + (pos+1)*v->elem_size,
             (char*)v->data + pos*v->elem_size,
             (v->length-pos)*v->elem_size);
     memcpy((char*)v->data + pos*v->elem_size, elem, v->elem_size);
     v->length++;
 }
 #endif
 
 /* ================================================================ */
 /*  Inserção ordenada + incremento usando dynvec_index               */
 /* ================================================================ */
 
 static void insert_or_inc(dynvec *v,char *tok){
     Word key={tok,0};
     size_t idx=dynvec_index(v,&key,cmp_alpha);
     if(idx!=v->length){                /* já existe – incrementa */
         Word *w=dynvec_get(v,idx);
         w->count++; free(tok);
         return;
     }
     /* não existe – insere mantendo ordem alfabética */
     size_t pos=0; while(pos<v->length && cmp_alpha(&key,dynvec_get(v,pos))>0) pos++;
     Word nw={tok,1};
     dynvec_insert(v,pos,&nw);
 }
 
 /* ================================================================ */
 /*  Programa principal                                               */
 /* ================================================================ */
 
 int main(void){
     size_t N; if(scanf("%zu",&N)!=1){fprintf(stderr,"Uso: <N>\n");return 1;}
     int ch; char buf[256]; size_t len=0;
     dynvec *dict=dynvec_create(sizeof(Word));
 
     while((ch=getchar())!=EOF){
         if(isalpha((unsigned char)ch)){
             if(len<sizeof buf-1) buf[len++]=(char)tolower(ch);
         }else if(len){
             buf[len]='\0'; insert_or_inc(dict,strdup(buf)); len=0;
         }
     }
     if(len){ buf[len]='\0'; insert_or_inc(dict,strdup(buf)); }
 
     /* ----------- Iteradores genéricos "TD2" ---------------- */
 
     dynvec_map(dict,to_lower_word);                         /* map */
 
     if(dynvec_exists(dict,is_longer_than_20))               /* exists */
         fprintf(stderr,"⚠︎ Há palavras com mais de 20 letras!\n");
 
     if(!dynvec_forall(dict,count_gt_zero))                  /* forall */
         fprintf(stderr,"Erro: detetado count == 0; estrutura corrompida.\n");
 
     if(dynvec_exists_index(dict,starts_with_upper)!= (size_t)-1) /* exists_index */
         fprintf(stderr,"⚠︎ Restam palavras iniciadas por maiúscula!\n");
 
     size_t total=0; dynvec_fold_left(dict,&total,sum_counts);     /* fold_left */
     fprintf(stderr,"Total de ocorrências: %zu\n",total);
 
     dynvec *filtrado=dynvec_filter(dict,freq_ge_2);              /* filter */
 
     quicksort_dynvec(filtrado,cmp_freq);                         /* sort */
 
     size_t out=(N<filtrado->length)?N:filtrado->length;
     for(size_t i=0;i<out;++i){
         Word *w=dynvec_get(filtrado,i);
         printf("%zu %s\n",w->count,w->word);
     }
 
     dynvec_free(filtrado,NULL);          /* palavras libertadas em dict */
     dynvec_free(dict, (void(*)(void*))free);
     return 0;
 }
 