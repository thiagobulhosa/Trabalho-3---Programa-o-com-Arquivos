//Thiago Dos Santos
//Thiago Bulhosa
//Marina Bulhosa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

typedef struct Ocorrencia{
int arquivo; //posição do arquivo na lista de arquivos processados.
int qtdOcorrencias; //qtd de vezes em que a palavra foi encontrada
int *linhas; //linhas onde a palavra foi encontrada
struct Ocorrencia *prox; //ponteiro para a próxima ocorrência da palavra em outro arquivo
}Ocorrencia;

typedef struct Palavra{
char letras[50]; //a palavra em questão
int qtdOcorrencias; //quantidade de arquivos que possuem a palavra
struct Ocorrencia *ocorrencias; //lista contendo informações da ocorrência de uma palavra em

//um arquivo

struct Palavra *prox;//ponteiro para a próxima palavra encontrada em qualquer arquivo (em

//ordem alfabética)

}Palavra;

typedef struct Arquivo{
char nomeArquivo[50]; //nome de um arquivo texto já processado
struct Arquivo *prox; //ponteiro para o nome do próximo arquivo texto
}Arquivo;

typedef struct Indice
{
int qtdArquivos; //quantidade de arquivos de texto lidos até então
struct Arquivo *arquivos; //lista contendo os nomes dos arquivos texto já processados
int qtdPalavras;
struct Palavra *iniciais[26];
struct Palavra *palavras;//lista contendo todas as palavras já encontrada
}Indice;

typedef struct vetor
{
  int num;
  struct vetor *prox;
}Vetor;

Indice *indice=NULL;

int verificaIndice(char *nomeArq)
{
  Indice *aux=indice;
  Arquivo *arq=aux->arquivos;
  if (aux->qtdArquivos==0)
    return 0;

  else
  {
    while(arq!=NULL)
    {
      if(strcmp(arq->nomeArquivo,nomeArq)==0)
        return 1;
      arq= arq->prox;
    }
  }
  return 0;
}

void insereArquivo(char *nomeArq)
{
  Arquivo *arq=indice->arquivos;
  Arquivo *novo=(Arquivo *)malloc(sizeof(Arquivo));
  strcpy(novo->nomeArquivo,nomeArq);
  novo->prox=NULL;

  if (arq==NULL)
  {
      indice->arquivos=novo;
  }

  else
  {
      while (arq->prox!=NULL)
      {
          arq=arq->prox;

      }
      arq->prox=novo;
  }
}

int verificaDicionario(char *palavra)
{
  int pos=palavra[0]-97;
  Palavra *auxiliar=indice->iniciais[pos];
  if (indice->qtdPalavras==0)
    return 0;

  else
  {
      while(auxiliar!=NULL)
      {
        if(strcmp(auxiliar->letras,palavra)==0)
        {
          return 1;
        }
        auxiliar = auxiliar->prox;
      }
  }
  return 0;
}

void insereOrdenadaDic(Palavra *NovaPalavra,int pos)
{
  Palavra *aux;

  if (indice->iniciais[pos]==NULL)
  {
    NovaPalavra->prox=NULL;
    indice->iniciais[pos]=NovaPalavra;
  }

  else if(strcmp(NovaPalavra->letras,indice->iniciais[pos]->letras)<0)
    {
        NovaPalavra->prox = indice->iniciais[pos];
        indice->iniciais[pos] = NovaPalavra;
    }

    else
    {
        aux = indice->iniciais[pos];
        while(aux->prox && (strcmp(NovaPalavra->letras,aux->prox->letras)>0))
            aux= aux->prox;
        NovaPalavra->prox = aux->prox;
        aux->prox = NovaPalavra;
    }
}

void addPalavraDicionario(char *palavra,int Nlinha,int posArq,int pos)
{
  Palavra *NovaPalavra;
  NovaPalavra=(Palavra *)malloc(sizeof(Palavra));
  strcpy(NovaPalavra->letras, palavra);
  NovaPalavra->qtdOcorrencias = 0;
  NovaPalavra->prox = NULL;
  insereOrdenadaDic(NovaPalavra,pos);
}

void inserePalavraOrdenada(Palavra *NovaPalavra)
{

    Palavra *aux;

    if (indice->palavras==NULL)
    {
      NovaPalavra->prox=NULL;
      indice->palavras=NovaPalavra;
    }

    else if(strcmp(NovaPalavra->letras,indice->palavras->letras)<0)
    {
        NovaPalavra->prox = indice->palavras;
        indice->palavras = NovaPalavra;
    }

    else
    {
        aux = indice->palavras;
        while(aux->prox && (strcmp(NovaPalavra->letras,aux->prox->letras)>0))
            aux= aux->prox;
        NovaPalavra->prox = aux->prox;
        aux->prox = NovaPalavra;
    }
}

void addPalavra(char *palavra,int Nlinha,int posArq)
{
  Palavra *NovaPalavra;
  NovaPalavra=(Palavra *)malloc(sizeof(Palavra));
  strcpy(NovaPalavra->letras, palavra);
  NovaPalavra->qtdOcorrencias = 0;
  NovaPalavra->prox = NULL;
  inserePalavraOrdenada(NovaPalavra);
}

void insereOcorrenciaDic(char *palavra,int Nlinha,int posArq,int pos)
{
    int flag=0;
    Palavra *auxPalavras=indice->iniciais[pos];
    Ocorrencia *aux2;

    while (auxPalavras!=NULL)
    {
      if (strcmp(auxPalavras->letras,palavra)==0)
      {

        aux2=auxPalavras->ocorrencias;
        if (aux2==NULL)
        {
          flag = 1;
          auxPalavras->qtdOcorrencias++;
          Ocorrencia *nova=(Ocorrencia *)malloc(sizeof(Ocorrencia));
          nova->qtdOcorrencias=1;
          nova->arquivo=posArq;
          nova->linhas=(int *)malloc(sizeof(int));
          nova->linhas[0]=Nlinha;
          nova->prox=NULL;
          auxPalavras->ocorrencias=nova;
        }

        else
        {
          while(aux2!=NULL)
          {
            if(aux2->arquivo==posArq)
            {
              flag = 1;
              aux2->qtdOcorrencias++;
              aux2->linhas=(int *)realloc(aux2->linhas,aux2->qtdOcorrencias*sizeof(int));
              aux2->linhas[aux2->qtdOcorrencias-1]=Nlinha;
            }
            aux2=aux2->prox;
          }


          if(flag==0)
          {
            auxPalavras->qtdOcorrencias++;
            Ocorrencia *nova=(Ocorrencia *)malloc(sizeof(Ocorrencia));
            nova->qtdOcorrencias=1;
            nova->arquivo=posArq;
            nova->linhas=(int *)malloc(sizeof(int));
            nova->linhas[0]=Nlinha;
            nova->prox=NULL;

            aux2=auxPalavras->ocorrencias;
            while(aux2->prox!=NULL){
              aux2=aux2->prox;
            }
            aux2->prox=nova;
          }



        }
      }
      auxPalavras=auxPalavras->prox;
    }
}

void insereOcorrencia(char *palavra,int Nlinha,int posArq)
{
    int flag=0;
    Palavra *auxPalavras=indice->palavras;
    Ocorrencia *aux2;

    while (auxPalavras!=NULL)
    {
      if (strcmp(auxPalavras->letras,palavra)==0)
      {

        aux2=auxPalavras->ocorrencias;
        if (aux2==NULL)
        {
          flag = 1;
          auxPalavras->qtdOcorrencias++;
          Ocorrencia *nova=(Ocorrencia *)malloc(sizeof(Ocorrencia));
          nova->qtdOcorrencias=1;
          nova->arquivo=posArq;
          nova->linhas=(int *)malloc(sizeof(int));
          nova->linhas[0]=Nlinha;
          nova->prox=NULL;
          auxPalavras->ocorrencias=nova;
        }

        else
        {
          while(aux2!=NULL)
          {
            if(aux2->arquivo==posArq)
            {
              flag = 1;
              aux2->qtdOcorrencias++;
              aux2->linhas=(int *)realloc(aux2->linhas,aux2->qtdOcorrencias*sizeof(int));
              aux2->linhas[aux2->qtdOcorrencias-1]=Nlinha;
            }
            aux2=aux2->prox;
          }


          if(flag==0)
          {
            auxPalavras->qtdOcorrencias++;
            Ocorrencia *nova=(Ocorrencia *)malloc(sizeof(Ocorrencia));
            nova->qtdOcorrencias=1;
            nova->arquivo=posArq;
            nova->linhas=(int *)malloc(sizeof(int));
            nova->linhas[0]=Nlinha;
            nova->prox=NULL;

            aux2=auxPalavras->ocorrencias;
            while(aux2->prox!=NULL){
              aux2=aux2->prox;
            }
            aux2->prox=nova;
          }



        }
      }
      auxPalavras=auxPalavras->prox;
    }
}

void leArquivo (int posArq,FILE *arq)
{
    int compara=0,cont=1,procura=-1,ind;
    char palavraArq[1000],*token;
    while (fgets(palavraArq, 1000,arq)!=NULL)
    {
        token=strtok(palavraArq," ,.!\n\0");
        while (token!=NULL)
        {
            for(int i = 0; i<strlen(token); i++)
            {
                token[i] = tolower(token[i]);
            }
            ind=token[0]-97;
            compara=verificaDicionario(token);
            if (compara==0)
            {
                addPalavra(token,cont,posArq);insereOcorrencia(token,cont,posArq);
                indice->qtdPalavras++;
                addPalavraDicionario(token,cont,posArq,ind);
            }
            else
              insereOcorrencia(token,cont,posArq);
              insereOcorrenciaDic(token,cont,posArq,ind);

            token=strtok(NULL," ,.!\n\0");
        }
        cont++;
    }
    fclose(arq);
}

void recuperaIndice()
{
    int tamanhoTitulo=0;
    int tamanhoPalavra=0;
    Palavra *auxPalavra;
    Arquivo *auxArq;
    Ocorrencia *auxOcr;
    FILE *dat;
    dat=fopen("indice.dat","rb");
    if (dat==NULL)
        printf("Erro!Nao é possivel abrir o arquivo :(\n");
    else
    {
        fread(&indice->qtdArquivos,sizeof(int),1,dat); //qtd arquivos processados
        printf("Arquivos processados: %d\n",indice->qtdArquivos);

        for (int i=0; i<indice->qtdArquivos; i++)
        {
            if(indice->arquivos==NULL){
              indice->arquivos=(Arquivo*) malloc(sizeof(Arquivo));
              fread(&tamanhoTitulo,sizeof(int),1,dat);
              printf("Tamanho do título do arquivo: %d\n", tamanhoTitulo);
              fread(&indice->arquivos->nomeArquivo,tamanhoTitulo*sizeof(char),1,dat);
              printf("Nome do arquivo: %s\n", indice->arquivos->nomeArquivo);
            }
            else{
              auxArq=indice->arquivos;
              while(auxArq->prox!=NULL){
                auxArq=auxArq->prox;
              }
              auxArq=(Arquivo*) malloc(sizeof(Arquivo));
              fread(&tamanhoTitulo,sizeof(int),1,dat);
              printf("Tamanho do título do arquivo: %d\n", tamanhoTitulo);
              fread(&auxArq->nomeArquivo,tamanhoTitulo*sizeof(char),1,dat);
              printf("Nome do arquivo: %s\n", auxArq->nomeArquivo);
            }
        }
    fread(&indice->qtdPalavras,sizeof(int),1,dat); //quantidade de palavras diferentes encontradas
    printf("Qtd palavras diferentes: %d\n", indice->qtdPalavras);

    for (int i = 0; i < indice->qtdPalavras; i++){
      if(indice->palavras==NULL)
      {
        indice->palavras=(Palavra*) malloc(sizeof(Palavra));
        fread (&tamanhoPalavra,sizeof(int),1,dat); //tamanho da palavra
        fread (&auxPalavra->letras, tamanhoPalavra*sizeof(char),1,dat);//pega a palavra em si
        fread (&indice->palavras->qtdOcorrencias, sizeof(int),1,dat); //qtd ocorrencias da palavra ao longo dos arquivos
        
        for(int i=0;i<indice->palavras->qtdOcorrencias;i++){
          if(indice->palavras->ocorrencias==NULL){

            indice->palavras->ocorrencias=(Ocorrencia*)malloc(sizeof(Ocorrencia));
            fread (&indice->palavras->ocorrencias->arquivo, sizeof(int),1,dat);//numero do arquivo
            fread (&indice->palavras->ocorrencias->qtdOcorrencias, sizeof(int),1,dat);//quantidade ocorrencias
            indice->palavras->ocorrencias->linhas = (int*) malloc(indice->palavras->ocorrencias->qtdOcorrencias*sizeof(int));
                for (int k=0; k<indice->palavras->ocorrencias->qtdOcorrencias; k++){
                    fread (&indice->palavras->ocorrencias->linhas[i], sizeof(int),1,dat); //Pega o total de linhas
                }
          }else{
            auxOcr=indice->palavras->ocorrencias;
            while(auxOcr->prox!=NULL){
              auxOcr=auxOcr->prox;
            }
            auxOcr=(Ocorrencia*)malloc(sizeof(Ocorrencia));
            fread (&auxOcr->arquivo, sizeof(int),1,dat);//numero do arquivo
            fread (&auxOcr->qtdOcorrencias, sizeof(int),1,dat);//quantidade ocorrencias
            auxOcr->linhas = (int*) malloc(auxOcr->qtdOcorrencias*sizeof(int));
                for (int k=0; k<auxOcr->qtdOcorrencias; k++){
                    fread (&auxOcr->linhas[i], sizeof(int),1,dat); //Pega o total de linhas
                }
          }
        }


      }
      else{
        auxPalavra=indice->palavras;
        while(auxPalavra->prox!=NULL)
        {
          auxPalavra=auxPalavra->prox;
        }
        auxPalavra=(Palavra*) malloc(sizeof(Palavra));
        fread (&tamanhoPalavra,sizeof(int),1,dat); //tamanho da palavra
        fread (&auxPalavra->letras, tamanhoPalavra*sizeof(char),1,dat);//pega a palavra em si
        fread (&auxPalavra->qtdOcorrencias, sizeof(int),1,dat); //qtd ocorrencias da palavra ao longo dos arquivos

        for(int i=0;i<auxPalavra->qtdOcorrencias;i++){
          if(auxPalavra->ocorrencias==NULL){

            auxPalavra->ocorrencias=(Ocorrencia*)malloc(sizeof(Ocorrencia));
            fread (&auxPalavra->ocorrencias->arquivo, sizeof(int),1,dat);//numero do arquivo
            fread (&auxPalavra->ocorrencias->qtdOcorrencias, sizeof(int),1,dat);//quantidade ocorrencias
            auxPalavra->ocorrencias->linhas = (int*) malloc(auxPalavra->ocorrencias->qtdOcorrencias*sizeof(int));
                for (int k=0; k<auxPalavra->ocorrencias->qtdOcorrencias; k++){
                    fread (&auxPalavra->ocorrencias->linhas[i], sizeof(int),1,dat); //Pega o total de linhas
                }
          }else{
            auxOcr=auxPalavra->ocorrencias;
            while(auxOcr->prox!=NULL){
              auxOcr=auxOcr->prox;
            }
            auxOcr=(Ocorrencia*)malloc(sizeof(Ocorrencia));
            fread (&auxOcr->arquivo, sizeof(int),1,dat);//numero do arquivo
            fread (&auxOcr->qtdOcorrencias, sizeof(int),1,dat);//quantidade ocorrencias
            auxOcr->linhas = (int*) malloc(auxOcr->qtdOcorrencias*sizeof(int));
                for (int k=0; k<auxOcr->qtdOcorrencias; k++){
                    fread (&auxOcr->linhas[i], sizeof(int),1,dat); //Pega o total de linhas
                }
          }
        }

      }      
    }
    }
        fclose(dat);
}

void escreveIndice(){
    FILE *dat;
    dat=fopen("indice.dat","w");

    int tamanhoTitulo = 0, tamanhoPalavras = 0;

    fwrite(&indice->qtdArquivos,sizeof(int),1,dat); //letra b

    Arquivo *auxTitulo=indice->arquivos;
    Palavra *auxPalavra=indice->palavras;
    Ocorrencia *auxOcorrencia;

    while (auxTitulo!=NULL){//letra c
        tamanhoTitulo = strlen(auxTitulo->nomeArquivo)+1;
        fwrite(&tamanhoTitulo,sizeof(int),1,dat); //qtd letras titulo
        fwrite(&auxTitulo->nomeArquivo,tamanhoTitulo*sizeof(char),1,dat); //nome do arquivo em questao
        auxTitulo=auxTitulo->prox;
    }

    fwrite(&indice->qtdPalavras,sizeof(int),1,dat); //letra d

     while (auxPalavra!=NULL){ //letra e
        tamanhoPalavras = strlen(auxPalavra->letras)+1;
        fwrite(&tamanhoPalavras,sizeof(int),1,dat);//qtd letras da palavra
        fwrite(&auxPalavra->letras,tamanhoPalavras*sizeof(char),1,dat);//letras da palavra
        fwrite(&auxPalavra->qtdOcorrencias,sizeof(int),1,dat);//qtd ocorrencias
        auxOcorrencia = auxPalavra->ocorrencias;
        while (auxOcorrencia!=NULL){ //letra f
            fwrite(&auxOcorrencia->arquivo,sizeof(int),1,dat); //numero do arquivo da ocorrencia
            fwrite(&auxOcorrencia->qtdOcorrencias,sizeof(int),1,dat); //quantidade de vezes que a palavra ocorreu
            for(int i=0; i<auxOcorrencia->qtdOcorrencias; i++){  //numero das linhas das palavras
                fwrite(&auxOcorrencia->linhas[i],sizeof(int),1,dat);
            }
        auxOcorrencia=auxOcorrencia->prox;
        }
        auxPalavra=auxPalavra->prox;
     }
    fclose(dat);
}

void limpaIndice(){

    Palavra *auxPalavra,*auxInicial;
    Arquivo *auxArquivo;
    Ocorrencia *auxOcorrencia;

    while(indice->arquivos !=NULL)
    {
        auxArquivo=indice->arquivos->prox;
        free(indice->arquivos);
        indice->arquivos=auxArquivo;
    }
    while(indice->palavras !=NULL)
    {
        auxPalavra=indice->palavras->prox;
        while (indice->palavras->ocorrencias){
            auxOcorrencia=indice->palavras->ocorrencias->prox;
            free (indice->palavras->ocorrencias->linhas);
            free(indice->palavras->ocorrencias);
            indice->palavras->ocorrencias=auxOcorrencia;
        }
        free(indice->palavras);
        indice->palavras=auxPalavra;
    }

    for(int i=0;i<26;i++)
    {
      while(indice->iniciais[i]!=NULL)
      {
        auxInicial=indice->iniciais[i]->prox;
        while (indice->iniciais[i]->ocorrencias){
            auxOcorrencia=indice->iniciais[i]->ocorrencias->prox;
            free (indice->iniciais[i]->ocorrencias->linhas);
            free(indice->iniciais[i]->ocorrencias);
            indice->iniciais[i]->ocorrencias=auxOcorrencia;
        }
        free(indice->iniciais[i]);
        indice->iniciais[i]=auxInicial;
      }
    }
}

void limpaLista(){

    Palavra *auxPalavra,*auxInicial;
    Ocorrencia *auxOcorrencia;

    while(indice->palavras !=NULL)
    {
        auxPalavra=indice->palavras->prox;
        while (indice->palavras->ocorrencias){
            auxOcorrencia=indice->palavras->ocorrencias->prox;
            free (indice->palavras->ocorrencias->linhas);
            free(indice->palavras->ocorrencias);
            indice->palavras->ocorrencias=auxOcorrencia;
        }
        free(indice->palavras);
        indice->palavras=auxPalavra;
    }
}

void carregaDicionario()
{
  int flag=0,atual=0,verifica;
  char *palavra;
  int *linhas,qtdOcorrencias=0,qtdOcorrenciaArquivo=0,posArquivo=0;

  while (indice->palavras!=NULL)
  {
    atual = indice->palavras->letras[0];
    //printf ("Palavra: %s Letra: %c\n", indice->palavras->letras, atual);
    indice->iniciais[atual-97] = (Palavra *)malloc (sizeof(Palavra));
    strcpy(palavra,indice->palavras->letras);
    qtdOcorrenciaArquivo = indice->palavras->qtdOcorrencias;

    while (indice->palavras->ocorrencias != NULL)
    {
        indice->iniciais[atual-97]->ocorrencias = (Ocorrencia*) malloc((sizeof(Ocorrencia)));
        posArquivo = indice->palavras->ocorrencias->arquivo;
        qtdOcorrencias=indice->palavras->ocorrencias->qtdOcorrencias;
        linhas = (int*) malloc (indice->iniciais[atual-97]->ocorrencias->qtdOcorrencias*sizeof(int));
        //printf ("Arq: %d Qtd:%d Linhas: ", indice->iniciais[atual-97]->ocorrencias->arquivo, indice->iniciais[atual-97]->ocorrencias->qtdOcorrencias);
        for (int k=0; k<indice->palavras->ocorrencias->qtdOcorrencias; k++)
        {
            linhas[k]=indice->palavras->ocorrencias->linhas[k];
            //printf ("%d", indice->iniciais[atual-97]->ocorrencias->linhas[k]);
        }
        indice->palavras->ocorrencias=indice->palavras->ocorrencias->prox;
    }
    int pos=palavra[0]-97;
    int U=0;
    verifica=verificaDicionario(palavra);
    if (verifica==0)
    {
      addPalavraDicionario(palavra,linhas[U],posArquivo,pos);
      U++;
    }
    
    else
    {
      insereOcorrenciaDic(palavra,linhas[U],posArquivo,pos);
      U++;
    }
    indice->palavras=indice->palavras->prox;
  }
}

Vetor *verificaTitulo(char *palavra1)
{
  int *arquivos;
  Vetor *vet=NULL;
  Vetor *novo;
  
  int pos=palavra1[0]-97,PosArquivo=0,cont=0,x;
  Arquivo *auxArq=indice->arquivos;
  Palavra *auxPalavra=indice->iniciais[pos];
  Ocorrencia *auxOcorrencia;
  while(auxPalavra!=NULL)
  {
    if(strcmp(auxPalavra->letras,palavra1)==0)
    {   

        auxOcorrencia=auxPalavra->ocorrencias;
        for(int i=0;i<auxPalavra->qtdOcorrencias;i++)
        {
          PosArquivo=auxOcorrencia->arquivo;
          novo=(Vetor *)malloc(sizeof(Vetor));
          novo->num=PosArquivo;
          novo->prox=vet;
          vet=novo;
          auxOcorrencia=auxOcorrencia->prox;
        }
    }
    auxPalavra = auxPalavra->prox;
  }
  return vet;
}

char *printaTitulo(int posArquivo)
{
  Arquivo *auxArq=indice->arquivos;
  for (int i=0;i<posArquivo;i++)
  {
    auxArq=auxArq->prox;
  }
  return auxArq->nomeArquivo;
}

void printaE(char *palavra1,char *palavra2)
{
  int flag=0;
  Vetor *verifica1,*verifica2;
  Vetor *aux1,*aux2,*aux3=NULL;
  char *nomeArquivo;
  verifica1=verificaTitulo(palavra1);
  verifica2=verificaTitulo(palavra2);
  aux1=verifica1;
  aux2=verifica2;

  while(aux1!=NULL)
  {
    while(aux2!=NULL)
    {
      if (aux1->num==aux2->num)
      {
        Vetor *novo=(Vetor *)malloc(sizeof(Vetor));
        novo->num=aux1->num;
        novo->prox=aux3;
        aux3=novo;
      }
      aux2=aux2->prox;
    }
    aux2=verifica2;
    aux1=aux1->prox;
  }
  printf("%s e %s\n",palavra1,palavra2);
  while(aux3!=NULL) 
  {
    nomeArquivo=printaTitulo(aux3->num);
    printf("%s\n",nomeArquivo);
    aux3=aux3->prox;
  }
}

void printaOU(char *palavra1,char *palavra2)
{
  int flag=0;
  Vetor *verifica1,*verifica2,*aux1,*aux2,*aux3;
  char *nomeArquivo;
  verifica1=verificaTitulo(palavra1);
  verifica2=verificaTitulo(palavra2);
  aux1=verifica1;
  aux2=verifica2;
  
  while(aux1!=NULL)
  {
    while(aux2!=NULL)
    {
      if (aux1==aux2)
      {
        flag=1;
      }
      aux2=aux2->prox;
    }

    if (flag==0)
    {
      Vetor *novo=(Vetor *)malloc(sizeof(Vetor));
        novo->num=aux1->num;
        novo->prox=aux2;
        aux2=novo;
    }

    aux2=verifica2;
    aux1=aux1->prox;
  }

  printf("%s ou %s\n",palavra1,palavra2);

  while(aux2!=NULL)
  {
    nomeArquivo=printaTitulo(aux2->num);
    printf("%s\n",nomeArquivo);
    aux2=aux2->prox;
  }
}

void buscaSimples(char *palavra1) 
{
  int pos=palavra1[0]-97,PosArquivo=0;
  Arquivo *auxArq=indice->arquivos;
  Palavra *auxPalavra=indice->iniciais[pos];
  Ocorrencia *auxOcorrencia;
  while(auxPalavra!=NULL)
  {
    if(strcmp(auxPalavra->letras,palavra1)==0)
    { 
        printf("Palavra:%s\n",auxPalavra->letras);
        
        auxOcorrencia=auxPalavra->ocorrencias;
        while(auxOcorrencia!=NULL)
        {
          PosArquivo=auxOcorrencia->arquivo;
          for (int i=0;i<PosArquivo;i++)
          {
            auxArq=auxArq->prox;
          }
          printf("Nome arquivo: %s\n",auxArq->nomeArquivo);
          auxArq=indice->arquivos;
          printf("Linhas:");
          for(int i=0; i<auxOcorrencia->qtdOcorrencias; i++)
          {
            printf("%d ",(auxOcorrencia->linhas[i]));
          }
          printf("\n");
          auxOcorrencia=auxOcorrencia->prox;
        }
    }
    auxPalavra = auxPalavra->prox;
  }
}

void buscaComposta(char *palavra1,char *palavra2,int operador)
{
  if (operador==1)
    printaE(palavra1,palavra2);

  else if(operador==2)
    printaOU(palavra1,palavra2);
}

void printa2()
{
  Palavra *aux;
  char letra;
  for (int i=0;i<26;i++)
  {
    letra=i+97;
    printf("%c:\n",letra);
    aux=indice->iniciais[i];
    while (aux!=NULL)
    {
      printf("%s\n",aux->letras);
      aux=aux->prox;
    }
  }
}

int main()
{
    indice=(Indice *)malloc(sizeof(Indice));
    indice->qtdArquivos=0;
    indice->qtdPalavras=0;
    indice->arquivos=NULL;
    indice->palavras=NULL;
    setlocale(LC_ALL, "Portuguese");
    int op=0,procura=0, tipoBusca=0, escolhaOperador=0;
    char nomeArq[100];
    while (op!=5)
    {
        printf("\n1-Processar arquivo\n");
        printf("2-Salvar índice atual\n");
        printf("3-Ler índice\n");
        printf("4-Buscar índice\n");
        printf("5-Encerrar programa\n");
        printf("Digite uma opcao:");
        scanf("%d",&op);
        switch (op)
        {
        case 1:
            printf("Digite o nome do arquivo:");
            scanf("%s",nomeArq);
            FILE *arq;
            arq=fopen(nomeArq,"r");
            if (arq==NULL)
            {
                printf("Digite um arquivo valido");
            }
            else
            {
              if (verificaIndice(nomeArq)==0)
              {
                insereArquivo(nomeArq);
                indice->qtdArquivos++;
                leArquivo(indice->qtdArquivos-1,arq);
                printa2();
              }

              else
                printf("O arquivo ja foi lido\n");
            }
            break;

        case 2:
            escreveIndice();
            break;

        case 3:
            limpaIndice();
            recuperaIndice();
            carregaDicionario();
            limpaLista();
            break;

        case 4:
            while (tipoBusca!=3)
            {
              escolhaOperador=0;
              printf("1-Busca Simples\n");
              printf("2-Busca composta\n");
              printf("3-voltar\n");
              printf("Digite uma opcao:");
              scanf ("%d", &tipoBusca);
              if (tipoBusca == 1)
              {
                char palavra1[25];
                printf("Digite uma palavra:");
                scanf("%s",palavra1);
                buscaSimples(palavra1);
                tipoBusca=0;
              }
              else if (tipoBusca == 2)
              {
                char palavra1[25];
                char palavra2[25];
                printf("Digite a primeira palavra:");
                scanf("%s",palavra1);
                fflush(stdin);
                printf("Digite a segunda palavra:");
                scanf("%s",palavra2);
                while (escolhaOperador!=3)
                {
                  printf ("1-Operador E\n2-Operador OU\n");
                  printf("Digite uma opcao:");
                  scanf ("%d", &escolhaOperador);
                  if (escolhaOperador == 1){
                    buscaComposta(palavra1,palavra2,escolhaOperador);
                    escolhaOperador = 3;
                  }
                  else if (escolhaOperador==2)
                  {
                    buscaComposta(palavra1,palavra2,escolhaOperador);
                    escolhaOperador = 3;
                  }
                  else if (escolhaOperador < 0 || escolhaOperador > 2){
                    printf ("Digite um valor válido!\n");
                    escolhaOperador = 3;
                  }
                }
                tipoBusca=0;
              }
              else if(tipoBusca<0 || tipoBusca>3)
              {
                printf ("Digite um valor válido!\n");
                tipoBusca = 0;
              }
            }
            break;

        case 5:
            limpaIndice();
            break;

        default:
            printf("Digite uma opcao valida\n");
            break;
        }
    }
    return 0;
}