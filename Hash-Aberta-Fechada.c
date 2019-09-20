#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define TAM_INICIAL 500
#define MAX_FATOR 70

typedef struct elemCliente{
	int id;
	char nome[30];
	char valor[20];	
	int colisaoPos;
	struct elemCliente * prox; 
}TCliente;


typedef struct tabelaHash{
	int Tsize;
	float qtd;
	int TotalColisao;
    TCliente **vetCli;
}THash;

float calcFator(float qtd, int Tsize){
	return (qtd/Tsize)*100;
}

THash * criarHash(int size){
		THash * hash =  (THash *) malloc(sizeof(THash));
		if(hash!=NULL)
		{
			int i;
			
			hash->Tsize  =  size;
			hash->vetCli  = (TCliente **) malloc (size * sizeof(TCliente*));
			
			if(hash->vetCli  == NULL)
			{
				free(hash);
				return NULL;						
			}
			
			hash->qtd = 0;
			hash->TotalColisao = 0;
			for (i = 0; i<hash->Tsize; i++)
			{
				hash->vetCli[i] = NULL;				
			}	
		}
	return hash;
}

void lerArquivo(THash * hash,bool HF){
	
  FILE *arqTxt;
  char Linha[100];
  char *result;
  int i;
 
  // Abre um arquivo TEXTO para LEITURA
  arqTxt = fopen("DadosBancoOrigem.txt", "rt");
 	
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
      
     	quebraStr(hash, Linha, HF);	
	  }
      
      i++;
  }
  
  fclose(arqTxt);
  printf(" \n DADOS INSERIDOS COM SUCESSO!\n");
   
}


void imprimeHashFechada( THash * hash){
int i;
 
printf("=====================================  \n");
 for (i = 0; i<hash->Tsize; i++){
		if(hash->vetCli[i] == NULL){
 				printf("\nNULL\n");
 		}else{
 			
 			if(hash->vetCli[i]->prox == NULL){
 				printf(" %d  - %s - %s ",hash->vetCli[i]->id, hash->vetCli[i]->nome, hash->vetCli[i]->valor );
			 }else{
				TCliente *lista = hash->vetCli[i];
		
			printf("\nVETOR[%d] : %d - %s - %s ",i,lista->id, lista->nome, lista->valor );
				printf("===================== COLISAO ===================== \n");
				while (lista->prox != NULL) {
					lista = lista->prox;
					printf("\n-> %d - %s - %s",lista->id, lista->nome, lista->valor );
			     	
				}
				printf("\n==================================================== \n");
			 }
			 printf("\n");
 		}
	 }  
}


void quebraStr(THash * hash, char string[100], bool HF){
     char v[3][80];
     TCliente cliente;
     const char s[2] = "|";
     char* strQuebrada;
 	 int i = 0;
 	 
      strQuebrada = strtok(string, s);

      while( strQuebrada != NULL ) 
	  {

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
     
     if(HF == true){
     	addFech(hash, cliente);
	 }else{
	 
     	add(hash, cliente);
	 }
}

THash* expandeTHash(THash * hash, bool HF){
	
	THash * newhash = criarHash(hash->Tsize*2);

		int i ;

		printf("\n%d - %d\n",newhash->Tsize, hash->Tsize);
		printf("\n%f - %f\n",newhash->qtd, hash->qtd);
		
		 for (i = 1; i<hash->Tsize; i++){
		 
 				TCliente * aux = hash->vetCli[i];
 				TCliente cli;
 				if(HF==true){
 				
 					while(aux != NULL){
	 			
					cli.id =  aux->id;
					strcpy(cli.nome,aux->nome);
					strcpy(cli.valor,aux->valor);
					cli.prox = NULL;
					
					cli.colisaoPos = aux->colisaoPos;
						addFech(newhash, cli);
						aux = aux->prox;
					}	
					
				 }else{
				 	
				 	if(hash->vetCli[i] != NULL){
				 			cli.id =  aux->id;
							strcpy(cli.nome,aux->nome);
							strcpy(cli.valor,aux->valor);
							cli.prox = NULL;
					
							cli.colisaoPos = aux->colisaoPos;
							add(newhash, cli);
					 }
				 	
				 }
 						 		
 }

return newhash;
	
}

int posHash(int chave, int size){
	return (chave & 0x7FFFFFFF)%size;
}

int posHashLinear(int pos, int i, int size){
	return(( (pos+i) & 0x7FFFFFFF) % size);
	
}

void liberaHash(THash* ha){
	if(ha!=NULL){
		int i;
		for (i = 0; i <ha->Tsize; i++){
			if(ha->vetCli[i] != NULL){
				free(ha->vetCli[i]);
			}
		}
	
	}
	
}

void add(THash* hash, TCliente cli){

		if(calcFator(hash->qtd, hash->Tsize) > MAX_FATOR)
		{
			printf("\n ======= ATINGIU FATOR MAXIMO  %.1f =======\n",calcFator(hash->qtd, hash->Tsize) );
			printf("\n ======= QTD REGISTROS  %1.f =======\n",hash->qtd );
			THash * newhash =  expandeTHash(hash, false);	
		
			liberaHash(hash);
			*hash = *newhash;
			
			imprimeHashAberta(hash);
			printf("\n ======= ATINGIU FATOR MAXIMO  %.1f =======\n",calcFator(hash->qtd, hash->Tsize) );
			printf("\n ======= QTD REGISTROS  %1.f =======\n",hash->qtd );

		}
	

	if(hash ==NULL || hash->qtd == hash->Tsize){
		
		return;
	}
	int chave  = cli.id;
	int i, pos, newpos;
	
	pos = posHash(chave, hash->Tsize);
	
	for(i= 0; i<hash->Tsize; i++){
		
		newpos = posHashLinear(pos, i, hash->Tsize);
					
		if(hash->vetCli[newpos] == NULL){
		
			TCliente* novo;
			novo = (TCliente*) malloc (sizeof(TCliente));
			if(novo ==NULL){
				return;
			}
			cli.colisaoPos = 0;
			* novo = cli;
			
			hash->vetCli[newpos] = novo;
			hash->qtd++;
	
		   return;
		}else{
			hash->vetCli[newpos]->colisaoPos++;
			hash->TotalColisao ++;
			
		}
	}
	
}

void imprimeHashAberta( THash * hash){
 int i;
printf("===================================== \n");
 for (i = 0; i<hash->Tsize; i++){
		if(hash->vetCli[i] == NULL){
 				printf("\nNULL\n");
 		}else{
 			printf(" ======= POSICAO: %d =======",i);
 			printf("\n id : %d \n", hash->vetCli[i]->id);
 			printf(" nome : %s \n", hash->vetCli[i]->nome);
 			printf(" valor : %s \n", hash->vetCli[i]->valor);
 		}
 		
 }
    
}


void buscaHashFechada(THash * hash, int idChave){


	if(hash ==NULL){
		return ;
	}
	
	int i, pos;
	pos  = posHash(idChave, hash->Tsize);
		
		if(hash->vetCli[pos] == NULL){
			
			printf("\n Registro ID: %d nao encontrado!\n",idChave);
			return ;
		}
		
			TCliente * aux = hash->vetCli[pos];
		
			while(aux != NULL){
				if(aux->id ==  idChave){
						printf("\n ========== Busca Registro POS: %d ========== ", pos);
						printf("\n ID: %d ", aux->id);
						printf("\n NOME: %s ", aux->nome);
						printf("\n VALOR: %s ",aux->valor);
						printf("\n ========== FIM BUSCA ========== \n");
						return ;
				}
				aux = aux->prox;
			}	

}


void removeHashFechada(THash * hash, int idChave){

	if(hash ==NULL){
		return ;
	}
	
	int i, pos;
	pos  = posHash(idChave, hash->Tsize);
		
		if(hash->vetCli[pos] == NULL){
			
			printf("\n Registro ID: %d nao encontrado!\n",idChave);
			return ;
		}
		
			TCliente * aux = hash->vetCli[pos];
		
			while(aux != NULL){
				if(aux->id ==  idChave){
						printf("\n ========== Removendo ========== ", pos);
		
						free(hash->vetCli[pos]);
						hash->vetCli[pos] = NULL;
						
						printf("\n ========== Registro removido da POS: %d ========== \n", pos);
						
						return;	
				}
				aux = aux->prox;
			}	

}

void * buscaHashAberta(THash* hash, int idChave){
	if(hash ==NULL){
		return ;
	}
	
	int i, pos, newpos;
	pos  = posHash(idChave, hash->Tsize);
	
	for(i = 0;  i<hash->Tsize; i++){
		newpos = posHashLinear(pos, i , hash->Tsize);
		
		if(hash->vetCli[newpos] == NULL){
			printf("\n Regsitro ID: %d nao encontrado!\n",idChave);
			return ;
		}
		if(hash->vetCli[newpos]->id ==  idChave){
		//	TCliente *cli = (hash->vetCli[newpos]);
		printf("\n ========== Busca Registro POS: %d ========== ", newpos);
		printf("\n ID: %d ", hash->vetCli[newpos]->id);
		printf("\n NOME: %s ", hash->vetCli[newpos]->nome);
		printf("\n VALOR: %s ", hash->vetCli[newpos]->valor);
		printf("\n ========== FIM BUSCA ========== \n");
			
		}
	}

}


void removeElemHashAberta(THash* hash, int idChave){
		if(hash ==NULL){
		return ;
	}
	
	int i, pos, newpos;
	pos  = posHash(idChave, hash->Tsize);
	
	for(i = 0;  i<hash->Tsize; i++){
		newpos = posHashLinear(pos, i , hash->Tsize);
		
		if(hash->vetCli[newpos] == NULL){
			printf("\n Registro ID: %d nao encontrado!\n",idChave);
			return ;
		}
		if(hash->vetCli[newpos]->id ==  idChave){
		//	TCliente *cli = (hash->vetCli[newpos]);
		printf("\n ========== Removendo ========== ", newpos);
		
		free(hash->vetCli[newpos]);
		hash->vetCli[newpos] = NULL;
		
		printf("\n ========== Registro removido da POS: %d ========== \n", newpos);
		
		return;	
		}
	}
}

void  geraDados(THash * hash, bool HF){
		
		FILE * arq;
			
		if	(HF ==true){
			
		
			
            arq = fopen("HashFechada.txt", "r");
            if(arq==NULL)
            {
	 			arq = fopen("HashFechada.txt", "w");
                printf("Arquivo criado! \n\n");
                
            } 

	    arq=fopen("HashFechada.txt","w"); 
	   
	    	int i;
			for(i = 0; i<hash->Tsize; i++){
				
				if(hash->vetCli[i] != NULL){
					fprintf(arq,"\n  TOTAL DE COLISAO NA POSICAO [%d]: %d  \n",i, hash->vetCli[i]->colisaoPos );
					printf("\n  TOTAL DE COLISAO NA POSICAO [%d]: %d  \n",i, hash->vetCli[i]->colisaoPos );					
				}else{
					fprintf(arq,"\n  TOTAL DE COLISAO NA POSICAO [%d]: 0  \n",i);
					printf("\n  TOTAL DE COLISAO NA POSICAO [%d]: 0  \n",i);
				}
					
			}
		fprintf(arq,"\n ****\n TOTAL DE ELEMENTOS: %.0f \n****\n", hash->qtd);
		printf("\n ****\n TOTAL DE ELEMENTOS: %.0f \n****\n", hash->qtd);
			
		fprintf(arq,"\n ==============\n TOTAL DE COLISAO HASH: %d \n==============\n", hash->TotalColisao);
		printf("\n ==============\n TOTAL DE COLISAO HASH: %d \n==============\n", hash->TotalColisao);

 }else{
 		
		
            arq = fopen("HashAberta.txt", "r");
            if(arq==NULL)
            {
	 			arq = fopen("HashAberta.txt", "w");
                printf("Arquivo criado! \n\n");
                
            } 

	    arq=fopen("HashAberta.txt","w"); 
	   
	    	int i;
			for(i = 0; i<hash->Tsize; i++){
				
				if(hash->vetCli[i] != NULL){
					fprintf(arq,"\n  TOTAL DE COLISAO NA POSICAO [%d]: %d  \n",i, hash->vetCli[i]->colisaoPos );
					printf("\n  TOTAL DE COLISAO NA POSICAO [%d]: %d  \n",i, hash->vetCli[i]->colisaoPos );
								
				}else{
					fprintf(arq,"\n  TOTAL DE COLISAO NA POSICAO [%d]: 0  \n",i);
					printf("\n  TOTAL DE COLISAO NA POSICAO [%d]: 0  \n",i);
				}
					
			}
	fprintf(arq,"\n****\n TOTAL DE ELEMENTOS: %.0f \n****\n", hash->qtd);
	printf("\n****\n TOTAL DE ELEMENTOS: %.0f \n****\n", hash->qtd);
		
	fprintf(arq,"\n ==============\n TOTAL DE COLISAO HASH: %d \n==============\n", hash->TotalColisao);
	printf("\n ==============\n TOTAL DE COLISAO HASH: %d \n==============\n", hash->TotalColisao);

 }
     printf("\n Arquivo gerado com sucesso!\n");     
	 
	 fclose(arq);  
	
}

void geraRegistroArq(THash * hash, bool HF){

		FILE * arq;
			
		if	(HF ==true){
			
            arq = fopen("RegistrosHashFechada.txt", "r");
            if(arq==NULL)
            {
	 			arq = fopen("RegistrosHashFechada.txt", "w");
                printf("Arquivo criado! \n\n");
                
            } 

	    arq=fopen("RegistrosHashFechada.txt","w"); 
	   
	    	int i;

			 for (i = 0; i<hash->Tsize; i++){
					if(hash->vetCli[i] != NULL){
			 			
			 			if(hash->vetCli[i]->prox == NULL){
			 			//	fprintf(arq,"%d;%s;%s\n",hash->vetCli[i]->id, hash->vetCli[i]->nome, hash->vetCli[i]->valor  );
			 			 	fprintf(arq,"%0.f;%d;%d\n",hash->qtd, hash->TotalColisao, hash->vetCli[i]->colisaoPos  );
						 }else{
							TCliente *lista = hash->vetCli[i];
						//  fprintf(arq,"%d;%s;%s\n",lista->id, lista->nome, lista->valor );
					    	fprintf(arq,"%0.f;%d;%d\n",hash->qtd, hash->TotalColisao,lista->colisaoPos);
						
							while (lista->prox != NULL) {
								lista = lista->prox;
							//	fprintf(arq, "%d;%s;%s\n",lista->id, lista->nome, lista->valor);
						     	fprintf(arq, "%0.f;%d;%d\n",hash->qtd, hash->TotalColisao, lista->colisaoPos);
							}
						
						 }
						
			 		}
				 }  
 }else{
 		
            arq = fopen("RegistrosHashAberta.txt", "r");
            if(arq==NULL)
            {
	 			arq = fopen("RegistrosHashAberta.txt", "w");
                printf("Arquivo criado! \n\n");
                
            } 

	    arq=fopen("RegistrosHashAberta.txt","w"); 
	   int i;
		 for (i = 0; i<hash->Tsize; i++){
				if(hash->vetCli[i] != NULL){		 		
		 			fprintf(arq, "%0.f;%d;%d \n", hash->qtd, hash->TotalColisao, hash->vetCli[i]->colisaoPos);
		 		}
		 		
		 }

 }
     printf("\n Arquivo gerado com sucesso!\n");     
	 
	 fclose(arq);  
	
}


void addFech(THash* hash, TCliente cli){
	
		TCliente* elemCli = (TCliente*) malloc(sizeof(TCliente));
		cli.colisaoPos = 0;
		cli.prox =NULL;
		*elemCli = cli;
	

	
		
		if(calcFator(hash->qtd, hash->Tsize) > MAX_FATOR )
		{
	
			printf("\n ======= ATINGIU FATOR MAXIMO  %.1f =======\n",calcFator(hash->qtd, hash->Tsize) );
			printf("\n ======= QTD REGISTROS  %1.f =======\n",hash->qtd );
			THash * newhash =  expandeTHash(hash, true);	
			
			liberaHash(hash);
			*hash = *newhash;
			
			imprimeHashFechada(hash);
			printf("\n ======= ATINGIU FATOR MAXIMO  %.1f =======\n",calcFator(hash->qtd, hash->Tsize) );
			printf("\n ======= QTD REGISTROS  %1.f =======\n",hash->qtd );

		}
		
	int pos  = posHash(cli.id, hash->Tsize);
	if (hash->vetCli[pos] !=NULL) { 
		
			TCliente * aux = hash->vetCli[pos];
			
			while(aux->prox != NULL){
				aux = aux->prox;
			}					
			
			aux->prox = elemCli;
			
			hash->vetCli[pos]->colisaoPos++;
			hash->TotalColisao ++;
			hash->qtd++;
			
			return;
	}else{
		
			hash->vetCli[pos] = elemCli;
			hash->qtd++;
	
		
	}
}

int dobra(int chave, int size){
	
	if (chave <=size){
		return chave;
	}
	int num_bits = 12;
	int parte1 = chave >> num_bits;
	int parte2 = chave & (size-1);
	return (parte1 ^ parte2);
}


void menuHFechada(THash *hashFech){
	int op = -1;int idChave, pos;
	TCliente cli;

	while(op!=0){
		printf("=============== HASH FECHADA ==============\n");
		printf("\n 1 - INSERIR DADOS ARQUIVO ");
		printf("\n 2 - INSERIR DADOS MANUAL ");
		printf("\n 3 - IMPRIMIR HASH ");
		printf("\n 4 - BUSCAR ELEMENTO HASH ");
		printf("\n 5 - REMOVER ELEMENTO HASH ");
		printf("\n 6 - COLISAO POSICAO[] - COLISAO TOTAL ");
		printf("\n 7 - GERAR ARQUIVO REGISTRO ");
		printf("\n 0 - <= VOLTAR  ");
		printf("\n========== FIM MENU HASH FECHADA =============\n");
		printf("\n\n Escolha uma opcao: ");
		scanf("%d",&op);
		switch(op){
		case 1:
			system("cls");
			lerArquivo(hashFech, true);
			geraDados(hashFech, true);
		break;
		
		case 2:				
			system("cls");
			printf("\n ID: ");
			scanf("%d",&cli.id);
			
			printf("\n NOME: ");
			scanf("%s",&cli.nome);
			
			printf("\n VALOR: ");
			scanf("%s",&cli.valor);
			cli.prox = NULL;
			cli.colisaoPos = 0;
			addFech(hashFech, cli);
			system("cls");
			printf("\n REGISTRO INSERIDO COM SUCESSO!\n\n");
		break;	
		case 3:
			system("cls");
		
			imprimeHashFechada(hashFech);
		break;	
		case 4:
			system("cls");
			printf("ID do elemento para BUSCA: ");
			scanf("%d",&idChave);
			buscaHashFechada(hashFech, idChave);	
		break;	
		
		case 5:
			system("cls");
			printf("ID do elemento para REMOVER: ");
			scanf("%d",&idChave);
			removeHashFechada(hashFech, idChave);
		break;
		
		case 6:
			system("cls");

			geraDados(hashFech, true);

		break;
		case 7:
			system("cls");
			geraRegistroArq(hashFech, true);
			
		break;
	
		}
	}
	
}


void menuHAberta(THash *hash){
	int op = -1;int idChave, pos;
	TCliente cli;

	while(op!=0){
		printf("=============== HASH ABERTA ==============\n");
		printf("\n 1 - INSERIR DADOS ARQUIVO ");
		printf("\n 2 - INSERIR DADOS MANUAL ");
		printf("\n 3 - IMPRIMIR HASH ");
		printf("\n 4 - BUSCAR ELEMENTO HASH ");
		printf("\n 5 - REMOVER ELEMENTO HASH ");
		printf("\n 6 - COLISAO POSICAO[]  -  COLISAO TOTAL");
		printf("\n 7 - GERAR ARQUIVO REGISTROS");
		printf("\n 0 - <= VOLTAR  ");
		printf("\n========== FIM MENU HASH ABERTA =============\n");
		printf("\n\n Escolha uma opcao: ");
		scanf("%d",&op);
		switch(op){
		case 1:
			system("cls");
			lerArquivo(hash, false);
			geraDados(hash, false);
		break;
		
		case 2:
			system("cls");
			printf("\n ID: ");
			scanf("%d",&cli.id);
			
			printf("\n NOME: ");
			scanf("%s",&cli.nome);
			
			printf("\n VALOR: ");
			scanf("%s",&cli.valor);
			add(hash, cli);
			system("cls");
			printf("\n REGISTRO INSERIDO COM SUCESSO!\n\n");
		break;	
		case 3:
			system("cls");
			imprimeHashAberta(hash);
		break;	
		case 4:
			system("cls");
			printf("ID do elemento para BUSCA: ");
			scanf("%d",&idChave);
			buscaHashAberta(hash, idChave);	
		break;	
		
		case 5:
			system("cls");
			printf("ID do elemento para REMOVER: ");
			scanf("%d",&idChave);
			removeElemHashAberta(hash, idChave);
		break;
		
		case 6:
			system("cls");
			geraDados(hash, false);
			
		break;
		case 7:
			system("cls");
			geraRegistroArq(hash, false);
			
		break;
	
		}
	}
	
}

int main(int argc, char *argv[]) {
	int op =-1;
	
	// Cria tabela hash
	THash * hash = criarHash(TAM_INICIAL);
	THash * hashFech = criarHash(TAM_INICIAL);
	
	while(op!=0){
		printf("=============== MENU ==============\n");
		printf("\n 1 - HASH FECHADA ");
		printf("\n 2 - HASH ABERTA ");
		printf("\n========== FIM MENU =============\n");
		printf("\n\n Escolha uma opcao: ");
		scanf("%d",&op);
		switch(op){
		case 1:
				system("cls");
			menuHFechada(hashFech);
		break;
		
		case 2:
			system("cls");
			menuHAberta(hash);
		break;	
		}
		
	}


}
