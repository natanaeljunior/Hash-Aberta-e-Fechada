#include <stdio.h>
#include <stdlib.h>



typedef struct cliente{
		int id;
		char nome[30];
		char valor[30];
}TCliente;

typedef struct elemento{
		TCliente cli;
		struct elemento * prox;
}TElem;

float factor = 0.0;
int TAM = 10;

void quebraStr(TElem* vetor,char string[100]);

int calcPhash(int a, int b){
	return a%b;
}

float calcFator(){
	return (factor/TAM)*100;
}


void add (TElem * vetor[TAM], TCliente  cliente ){
	TElem* elemCli = (TElem*) malloc(sizeof(TElem));
	elemCli->cli = cliente;
	elemCli->prox = NULL;
	
	
	if (vetor[calcPhash(cliente.id,TAM)]!=NULL) { 

			TElem * aux = vetor[calcPhash(cliente.id,TAM)];
			
			while(aux->prox != NULL){
				aux = aux->prox;
			}					
			aux->prox = elemCli;
			return;
	}else{
			vetor[calcPhash(cliente.id,TAM)] = elemCli;
	}
	
}

void addvetor2 (TElem * vetor[TAM], TCliente  cliente ){
	TElem* elemCli = (TElem*) malloc(sizeof(TElem));
	elemCli->cli = cliente;
	elemCli->prox = NULL;
			
	if(calcFator()>20.0){
		TElem * vetor3[TAM*3];
		
	}
	
	
	if (vetor[calcPhash(cliente.id,TAM)]!=NULL) { 
			int i;
			
			for(i = calcPhash(cliente.id,TAM); i<TAM;i++){
				if(vetor[i+1]==NULL){
					vetor[i+1] = elemCli;
					i=TAM;
					
				}
			}
	}else{
			vetor[calcPhash(cliente.id,TAM)] = elemCli;
	}
	factor= factor+1.0;

}

void imprimeVetor( TElem * vetor[TAM]){
 int i;
 
printf("=====================================  \n");
 for (i = 0; i<TAM; i++){
		if(vetor[i] == NULL){
 				printf("\nNULL\n");
 		}else{
 			
 			if(vetor[i]->prox == NULL){
 				printf("\n %d ",vetor[i]->cli.id);
			 }else{
				TElem *lista = vetor[i];
				while (lista != NULL) {
					printf(" %d ->",lista->cli.id);
			     	lista = lista->prox;
				}
			 }
			 printf("\n");
 		}
 }
    
}

void lerArquivo(TElem * vetor){
	
  FILE *arqTxt;
  char Linha[100];
  char *result;
  int i;
 
  // Abre um arquivo TEXTO para LEITURA
  arqTxt = fopen("DadosBancoPulini.txt", "rt");
 	
	if (arqTxt == NULL)
	{
	    printf("Problemas na LEITURA do arquivo\n");
	    return;
	}else{
		printf("\n LEITURA DO ARQUIVO OK! \n");
	}
	
  i = 1;
  while (!feof(arqTxt))
  {

     result =  fgets(Linha, 100, arqTxt);
      if (result) {
     	quebraStr(vetor, Linha);	
	  }
      
      i++;
  }
  
  fclose(arqTxt);
  printf(" \n DADOS INSERIDOS COM SUCESSO!\n");
   
}



void quebraStr(TElem* vetor,char string[100]){
     char v[3][80];
     TCliente cliente;
     const char s[2] = "|";
     char* strQuebrada;
 	 int i = 0;
 	 
      strQuebrada = strtok(string, s);

      while( strQuebrada != NULL ) {

    	if(i==0){
    		cliente.id = atoi(strQuebrada);
		}else
		if(i==1){
			 sprintf(cliente.nome, "%s", strQuebrada);
		}else
		if(i==2){
			sprintf(cliente.valor, "%s", strQuebrada);	
		}
	    i++;
      strQuebrada = strtok(NULL, s);
   }
     addvetor(vetor, cliente);
}


void inicializa(TElem **vetor) {
	int i;
	for(i=0; i<TAM; i++){
		 vetor[i]=NULL;		 
	}
		
}

int main(int argc, char *argv[]) {

	TElem * vetor[TAM];	
	inicializa(vetor);
	lerArquivo(vetor);
	imprimeVetor(vetor);
	
	return 0;
}

