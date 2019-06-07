/*
	DESENVOLVIDO POR:	Gustavo Lopes Pereira da Silva
						GustavoLopes899@gmail.com				  
	TODOS OS FLUXOGRAMAS DESTE JOGO ESTÃO ANEXADOS A ESTE ARQUIVO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio2.h>
#define tam 8

typedef struct jogadores {              // REGISTRO CONTENDO OS DADOS DOS JOGADORES //
	int pecas;
	int limite;
} jogadores;

typedef struct celula {                 // REGISTRO CONTENDO OS DADOS DO TABULEIRO //
	int peca;
	int dama;
} matriz;

void criarTabuleiro(matriz tabuleiro[][tam]);
void desenharTabuleiro(int jogador, jogadores jogador1, jogadores jogador2, matriz tabuleiro[][tam]);
void inicializar(int *jogador, jogadores *jogador1, jogadores *jogador2, matriz tabuleiro[][tam], int jogadas[][12]);
void zerarJogadas(int jogadas[][12]);
int testaLimite(int novaLinha, int novaColuna);
int testaDiagonal(int linhaAtual, int colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]);
int checarVencedor(int jogador, jogadores jogador1, jogadores jogador2);
int testaComer(int jogador, jogadores *jogador1, jogadores *jogador2, int *comeu, matriz tabuleiro[][tam], int *linhaAtual, int *colunaAtual, int jogadas[][12]);
void checarComer(int jogador, matriz tabuleiro[][tam], int *linhaAtual, int *colunaAtual, int linha, int coluna, int jogadas[][12], int comeu);
void checarComerDama(int jogador, matriz tabuleiro[][tam], int linhaAtual, int colunaAtual, int jogadas[][12], int *linhaAux, int *colunaAux);
int comer(int jogador, jogadores *jogador1, jogadores *jogador2, int *comeu, int *linhaAtual, int *colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]);
int comerDama(int jogador, jogadores *jogador1, jogadores *jogador2, int *comeu, int *linhaAtual, int *colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]);
int andar(int jogador, jogadores jogador1, jogadores jogador2, int linhaAtual, int colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]);
void checaVirouDama(int jogador, matriz tabuleiro[][tam], jogadores jogador1, jogadores jogador2, int novaLinha, int novaColuna);

// FLUXOGRAMA MAIN //
int main() {
	jogadores jogador1, jogador2;
	matriz tabuleiro[tam][tam];
	int jogador, comeu, jogadas[2][12], linhaAtual, colunaAtual, novaLinha, novaColuna, resultado, andou;

	// INICIALIZAÇÃO DOS DADOS //
	inicializar(&jogador, &jogador1, &jogador2, tabuleiro, jogadas);
	
	do {
		linhaAtual = 0;
		colunaAtual = 0;
		comeu = 0;
		while (testaComer(jogador, &jogador1, &jogador2, &comeu, tabuleiro, &linhaAtual, &colunaAtual, jogadas) != 0);
		if (comeu == 0) {
			do {
				system("cls");
				desenharTabuleiro(jogador, jogador1, jogador2, tabuleiro);
				printf("\nEntre com a linha da peça a se mover: ");
				scanf("%d", &linhaAtual);
				printf("Entre com a coluna da peça a se mover: ");
				scanf("%d", &colunaAtual);
				printf("\nEntre com a linha destino: ");
				scanf("%d", &novaLinha);
				printf("Entre com a coluna destino: ");
				scanf("%d", &novaColuna);
				andou = andar(jogador, jogador1, jogador2, linhaAtual, colunaAtual, novaLinha, novaColuna, tabuleiro);
				if (andou == 0) {
					printf("\nNão foi possivel andar, tente novamente...");
					getch();
				}
			} while (andou == 0);
		}
		system("cls");
		resultado = checarVencedor(jogador, jogador1, jogador2);
		if (jogador == 1) {
			jogador = 2;
		} else {
			jogador = 1;
		}
	} while (resultado == 0);
	printf("=====================\n");
	printf("O jogador %d ganhou!!\n", resultado);
	printf("=====================\n");
	getch();
}
// FIM FLUXOGRAMA MAIN //

// FLUXOGRAMA CRIARTABULEIRO //
void criarTabuleiro(matriz tabuleiro[][tam]) {              // Função para criar o tabuleiro
	for (int i = 0; i < tam; i++) {
		for (int j = 0; j < tam; j++) {
			tabuleiro[i][j].dama = 0;
			if (((i+1)%2==0 && (j+1)%2==0) || (i%2==0 && j%2==0)) {
				tabuleiro[i][j].peca = 0;
			} else {
				if (i<=2) {
					tabuleiro[i][j].peca = 2;           // INSERÇÃO DAS PEÇAS DO JOGADOR 2
				} else {
					if (i>=5) {
						tabuleiro[i][j].peca = 1;       // INSERÇÃO DAS PEÇAS DO JOGADOR 1
					} else {
						tabuleiro[i][j].peca = 0;       // INSERÇÃO DOS ESPAÇOS VAZIOS DISPONIVEIS PARA JOGAR
					}
				}
			}
		}
	}
}
// FIM FLUXOGRAMA CRIARTABULEIRO //

// FLUXOGRAMA DESENHARTABULEIRO //
void desenharTabuleiro(int jogador, jogadores jogador1, jogadores jogador2, matriz tabuleiro[][tam]) {      // Função responsável por impimir o tabuleiro
	textcolor(15);
	textbackground(0);
	printf("=====================================================================\n");
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DAMAS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	printf("=====================================================================\n\n");
	printf("    0  1  2  3  4  5  6  7\t\tRegras:\n\n");
	for (int i = 0; i < tam; i++) {
		textbackground(0);
		textcolor(15);
		printf("%d  ", i);
		for (int j = 0; j <= tam; j++) {

			/***********************************************************************************************/
			/*                         	IMPRESSÃO DAS REGRAS DO JOGO                                       */
			/***********************************************************************************************/

			if (j >= tam) {
				textbackground(0);
				textcolor(15);
			}
			if (i == 0 && j == tam) {
				printf("\tÉ possivel movimentar apenas uma peça por vez.");
			}
			if (i == 2 && j == tam) {
				printf("\tSe caso for possivel comer alguma peça,");
			}
			if (i == 3 && j == tam) {
				printf("\testa jogada é obrigatória.");
			}
			if (i == 5 && j == tam) {
				printf("\tGanha quem comer todas as peças do outro.");
			}
			if (i == 7 && j == tam) {
				printf("\tPeças: jog. 1 (verm): %d ; jog. 2 (amar): %d", jogador1.pecas, jogador2.pecas);
			}
			if (j == tam) {
				break;
			}

			/***********************************************************************************************/
			/*                              IMPRESSÃO DAS PEÇAS                                            */
			/***********************************************************************************************/

			if (((i+1)%2==0 && (j+1)%2==0) || (i%2==0 && j%2==0)) {
				textbackground(15);
			} else {
				textbackground(0);
			}
			if (tabuleiro[i][j].peca == 1) {
				textcolor(12);
				if (tabuleiro[i][j].dama == 1) {
					printf(" @ ");                          // PEÇA DAMA REPRESENTADA POR @ //
				} else {
					printf(" 0 ");                          // PEÇA NORMAL REPRESENTADA POR 0 //
				}
			} else {
				if (tabuleiro[i][j].peca == 2) {
					textcolor(14);
					if (tabuleiro[i][j].dama == 1) {
						printf(" @ ");                      // PEÇA DAMA REPRESENTADA POR @ //
					} else {
						printf(" 0 ");                      // PEÇA NORMAL REPRESENTADA POR 0 //
					}

				} else {
					if (j < tam) {
						printf("   ");
					}
				}
			}
		}
		printf("\n");
	}
	textbackground(0);
	textcolor(15);
	printf("\n\t>> JOGADOR %d:\n", jogador);                // INDICADOR DO JOGADOR ATUAL
}
// FIM FLUXOGRAMA DESENHARTABULEIRO //

// FLUXOGRAMA INICIALIZAR //
void inicializar(int *jogador, jogadores *jogador1, jogadores *jogador2, matriz tabuleiro[][tam], int jogadas[][12]) {      // Função responsável por inicializar os dados do tabuleiro
	setlocale(LC_ALL, "portuguese");
	*jogador = 1;
	(*jogador1).pecas = 12;
	(*jogador1).limite = 0;
	(*jogador2).pecas = 12;
	(*jogador2).limite = 7;
	criarTabuleiro(tabuleiro);
}
// FIM FLUXOGRAMA INICIALIZAR //

// FLUXOGRAMA ZERARJOGADAS //
void zerarJogadas(int jogadas[][12]) {                      // Função responsável por zerar o vetor de possibilidades de jogadas
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 12; j++) {
			jogadas[i][j] = -1;
		}
	}
}
// FIM FLUXOGRAMA ZERARJOGADAS //

// FLUXOGRAMA TESTALIMITE //
int testaLimite(int novaLinha, int novaColuna) {            // Função para testar se os limites do tabuleiro não foram ultrapassados
	if (novaLinha >= 0 && novaLinha <=7 && novaColuna >= 0 && novaColuna <=7) {
		return 1;
	} else {
		return 0;
	}
}
// FIM FLUXOGRAMA TESTALIMITE //

// FLUXOGRAMA TESTADIAGONAL //
int testaDiagonal(int linhaAtual, int colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]) {        // Função que testa a diagoral, usada para andar coma a dama
	// EXISTEM QUATRO POSSIBILIDADES DE DIAGONAIS PARA TESTAR //
	if (linhaAtual > novaLinha) {
		if (colunaAtual > novaColuna) {
			// 1 -> DIAGONAL PARA CIMA/ESQUERDA //
			while (tabuleiro[linhaAtual][colunaAtual].peca == 0 && testaLimite(linhaAtual, colunaAtual)) {
				linhaAtual--;
				colunaAtual--;
				if (testaLimite(linhaAtual, colunaAtual)) {
					if (tabuleiro[linhaAtual][colunaAtual].peca == 0) {
						if (linhaAtual == novaLinha && colunaAtual == novaColuna) {
							return 1;
						}
					}
				}
			}
		} else {
			// 2 -> DIAGONAL PARA CIMA/DIREITA //
			while (testaLimite(linhaAtual, colunaAtual)) {
				linhaAtual--;
				colunaAtual++;
				if (testaLimite(linhaAtual, colunaAtual)) {
					if (tabuleiro[linhaAtual][colunaAtual].peca == 0) {
						if (linhaAtual == novaLinha && colunaAtual == novaColuna) {
							return 1;
						}
					}
				}
			}
		}
	} else {
		if (colunaAtual < novaColuna) {
			// 3 -> DIAGONAL PARA BAIXO/DIREITA //
			while (testaLimite(linhaAtual, colunaAtual)) {
				linhaAtual++;
				colunaAtual++;
				if (testaLimite(linhaAtual, colunaAtual)) {
					if (tabuleiro[linhaAtual][colunaAtual].peca == 0) {
						if (linhaAtual == novaLinha && colunaAtual == novaColuna) {
							return 1;
						}
					}
				}
			}
		} else {
			// 4 -> DIAGONAL PARA BAIXO/ESQUERDA //
			while (testaLimite(linhaAtual, colunaAtual)) {
				linhaAtual++;
				colunaAtual--;
				if (testaLimite(linhaAtual, colunaAtual)) {
					if (tabuleiro[linhaAtual][colunaAtual].peca == 0) {
						if (linhaAtual == novaLinha && colunaAtual == novaColuna) {
							return 1;
						}
					}
				}
			}
		}
	}
}
// FIM FLUXOGRAMA TESTADIAGONAL //

// FLUXOGRAMA CHECARVENCEDOR //
int checarVencedor(int jogador, jogadores jogador1, jogadores jogador2) {       // Função que checa se já houve algum ganhador
	if (jogador == 1) {
		if (jogador2.pecas == 0) {
			return jogador;
		}
	} else {
		if(jogador1.pecas == 0) {
			return jogador;
		}
	}
	return 0;
}
// FIM FLUXOGRAMA CHECAR VENCEDOR //

// FLUXOGRAMA TESTACOMER //
int testaComer(int jogador, jogadores *jogador1, jogadores *jogador2, int *comeu, matriz tabuleiro[][tam], int *linhaAtual, int *colunaAtual, int jogadas[][12]) {      // Função que verifica se existe possibilidades do jogador comer outras peças
	int i = 0, j = 0, k, peca, novaLinha, novaColuna;
	zerarJogadas(jogadas);
	checarComer(jogador, tabuleiro, linhaAtual, colunaAtual, *linhaAtual, *colunaAtual, jogadas, *comeu);
	system("cls");
	desenharTabuleiro(jogador, *jogador1, *jogador2, tabuleiro);

	if (jogadas[0][0] != -1) {
		do {                                            // LAÇO RESPONSÁVEL POR COMER, ENQUANTO FOR POSSIVEL COMER E NÃO COMER, NÃO SAI
			peca = 1;
			j = 0;
			printf("\n>> As peças possiveis para serem movimentadas são:\n");
			while (jogadas[i][j] != -1) {
				printf("\nPeça %d: Linha %d e coluna %d\n", peca++, jogadas[i][j], jogadas[i+1][j]);
				j++;
			}
			k = 0;
			printf("\n>> Qual peça voce deseja movimentar:");
			printf("\n\nLinha: ");
			scanf("%d", linhaAtual);
			printf("Coluna: ");
			scanf("%d", colunaAtual);
			do {
				if (jogadas[i][k] == *linhaAtual && jogadas[i+1][k] == *colunaAtual) {
					system("cls");
					desenharTabuleiro(jogador, *jogador1, *jogador2, tabuleiro);
					printf("\n>> Peça atual: Linha: %d e Coluna: %d\n", *linhaAtual, *colunaAtual);
					printf("\n>> Para onde voce deseja movimentar:");
					printf("\n\nLinha: ");
					scanf("%d", &novaLinha);
					printf("Coluna: ");
					scanf("%d", &novaColuna);
					if (comer(jogador, jogador1, jogador2, comeu, linhaAtual, colunaAtual, novaLinha, novaColuna, tabuleiro) != 0) {
						*comeu = 1;
						break;
					}
				}
				k++;
			} while (jogadas[i][k] != -1);
			system("cls");
			desenharTabuleiro(jogador, *jogador1, *jogador2, tabuleiro);
		} while (*comeu == 0);
		return 1;
	} else {
		return 0;                   // CASO NÃO SEJA POSSIVEL COMER, RETURN 0
	}
}
// FIM FLUXOGRAMA TESTACOMER //

// FLUXOGRAMA CHECARCOMER //
void checarComer(int jogador, matriz tabuleiro[][tam], int *linhaAtual, int *colunaAtual, int linha, int coluna, int jogadas[][12], int comeu) {        // Função que salva as possibilidades de comer, utilizando peças normais, (se houver) no vetor
	// EXISTEM QUATRO POSSIBILIDADES DE DIAGONAIS PARA CHECAR //
	int cont;
	int novaLinha, novaColuna, linhaAux = 0, colunaAux = 0;
	for (int linha = 0; linha < tam; linha++) {
		for (int coluna = 0; coluna < tam; coluna++) {
			if (comeu) {
				linha = *linhaAtual;
				coluna = *colunaAtual;
			}
			cont = 1;
			if (tabuleiro[linha][coluna].peca != 0 && tabuleiro[linha][coluna].peca == jogador) {
				if (tabuleiro[linha][coluna].dama == 1) {
					// CHECAR COMER DAMA //
					checarComerDama(jogador, tabuleiro, linha, coluna, jogadas, &linhaAux, &colunaAux);
					return;
				} else {
					// CHECAR COMER PEÇA NORMAL //
					while (cont <= 4) {
						switch(cont) {
								// 1 -> DIAGONAL PARA CIMA/ESQUERDA //
							case 1: {
								novaLinha = linha - 1;
								novaColuna = coluna - 1;
								if (testaLimite(novaLinha, novaColuna)) {
									if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
										novaLinha = linha - 2;
										novaColuna = coluna - 2;
										if (testaLimite(novaLinha, novaColuna)) {
											if (tabuleiro[novaLinha][novaColuna].peca == 0) {
												jogadas[linhaAux][colunaAux] = linha;
												linhaAux++;
												jogadas[linhaAux][colunaAux] = coluna;
												colunaAux++;
												linhaAux = 0;
												cont = 5;
											} else {
												cont++;
											}
										} else {
											cont++;
										}
									} else {
										cont++;
									}
								} else {
									cont++;
								}
								break;
							}
							// 2 -> DIAGONAL PARA BAIXO/ESQUERDA //
							case 2: {
								novaLinha = linha + 1;
								novaColuna = coluna - 1;
								if (testaLimite(novaLinha, novaColuna)) {
									if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
										novaLinha = linha + 2;
										novaColuna = coluna - 2;
										if (testaLimite(novaLinha, novaColuna)) {
											if (tabuleiro[novaLinha][novaColuna].peca == 0) {
												jogadas[linhaAux][colunaAux] = linha;
												linhaAux++;
												jogadas[linhaAux][colunaAux] = coluna;
												colunaAux++;
												linhaAux = 0;
												cont = 5;
											} else {
												cont++;
											}
										} else {
											cont++;
										}
									} else {
										cont++;
									}
								} else {
									cont++;
								}
								break;
							}
							// 3 -> DIAGONAL PARA BAIXO/DIREITA //
							case 3: {
								novaLinha = linha + 1;
								novaColuna = coluna + 1;
								if (testaLimite(novaLinha, novaColuna)) {
									if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
										novaLinha = linha + 2;
										novaColuna = coluna + 2;
										if (testaLimite(novaLinha, novaColuna)) {
											if (tabuleiro[novaLinha][novaColuna].peca == 0) {
												jogadas[linhaAux][colunaAux] = linha;
												linhaAux++;
												jogadas[linhaAux][colunaAux] = coluna;
												colunaAux++;
												linhaAux = 0;
												cont = 5;
											} else {
												cont++;
											}
										} else {
											cont++;
										}
									} else {
										cont++;
									}
								} else {
									cont++;
								}
								break;
							}
							// 4 -> DIAGONAL PARA CIMA/DIREITA //
							case 4: {
								novaLinha = linha - 1;
								novaColuna = coluna + 1;
								if (testaLimite(novaLinha, novaColuna)) {
									if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
										novaLinha = linha - 2;
										novaColuna = coluna + 2;
										if (testaLimite(novaLinha, novaColuna)) {
											if (tabuleiro[novaLinha][novaColuna].peca == 0) {
												jogadas[linhaAux][colunaAux] = linha;
												linhaAux++;
												jogadas[linhaAux][colunaAux] = coluna;
												colunaAux++;
												linhaAux = 0;
												cont = 5;
											} else {
												cont++;
											}
										} else {
											cont++;
										}
									} else {
										cont++;
									}
								} else {
									cont++;
								}
								break;
							}
						}
					}
					if (comeu) {
						return;
					}
				}
			}
		}
	}
}
// FIM FLUXOGRAMA CHECARCOMER //

// FLUXOGRAMA CHECARCOMERDAMA //
void checarComerDama(int jogador, matriz tabuleiro[][tam], int linhaAtual, int colunaAtual, int jogadas[][12], int *linhaAux, int *colunaAux) {             // Função que salva as possibilidades de comer, utilizando peças dama, (se houver) no vetor
	// EXISTEM QUATRO POSSIBILIDADES DE DIAGONAIS CHECAR //
	int cont = 1, aux;
	int novaLinha, novaColuna;
	while (cont <= 4) {
		switch(cont) {
				// 1 -> DIAGONAL PARA CIMA/ESQUERDA //
			case 1: {
				aux = 1;
				do {
					novaLinha = linhaAtual - aux;
					novaColuna = colunaAtual - aux;
					if (testaLimite(novaLinha, novaColuna)) {
						aux++;
					} else {
						break;
					}
				} while (tabuleiro[novaLinha][novaColuna].peca == 0);
				if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
					novaLinha--;
					novaColuna--;
					if (testaLimite(novaLinha, novaColuna) && tabuleiro[novaLinha][novaColuna].peca == 0) {
						jogadas[*linhaAux][*colunaAux] = linhaAtual;
						(*linhaAux)++;
						jogadas[*linhaAux][*colunaAux] = colunaAtual;
						(*colunaAux)++;
						*linhaAux = 0;
						cont = 5;
					} else {
						cont++;
					}
				} else {
					cont++;
				}
				break;
			}
			// 2 -> DIAGONAL PARA CIMA/DIREITA //
			case 2: {
				aux = 1;
				do {
					novaLinha = linhaAtual - aux;
					novaColuna = colunaAtual + aux;
					if (testaLimite(novaLinha, novaColuna)) {
						aux++;
					} else {
						break;
					}
				} while (tabuleiro[novaLinha][novaColuna].peca == 0);
				if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
					novaLinha--;
					novaColuna++;
					if (testaLimite(novaLinha, novaColuna) && tabuleiro[novaLinha][novaColuna].peca == 0) {
						jogadas[*linhaAux][*colunaAux] = linhaAtual;
						(*linhaAux)++;
						jogadas[*linhaAux][*colunaAux] = colunaAtual;
						(*colunaAux)++;
						*linhaAux = 0;
						cont = 5;
					} else {
						cont++;
					}
				} else {
					cont++;
				}
				break;
			}
			// 3 -> DIAGONAL PARA BAIXO/DIREITA //
			case 3: {
				aux = 1;
				do {
					novaLinha = linhaAtual + aux;
					novaColuna = colunaAtual + aux;
					if (testaLimite(novaLinha, novaColuna)) {
						aux++;
					} else {
						break;
					}
				} while (tabuleiro[novaLinha][novaColuna].peca == 0);
				if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
					novaLinha++;
					novaColuna++;
					if (testaLimite(novaLinha, novaColuna) && tabuleiro[novaLinha][novaColuna].peca == 0) {
						jogadas[*linhaAux][*colunaAux] = linhaAtual;
						(*linhaAux)++;
						jogadas[*linhaAux][*colunaAux] = colunaAtual;
						(*colunaAux)++;
						*linhaAux = 0;
						cont = 5;
					} else {
						cont++;
					}
				} else {
					cont++;
				}
				break;
			}
			// 4 -> DIAGONAL PARA BAIXO/ESQUERDA //
			case 4: {
				aux = 1;
				do {
					novaLinha = linhaAtual + aux;
					novaColuna = colunaAtual - aux;
					if (testaLimite(novaLinha, novaColuna)) {
						aux++;
					} else {
						break;
					}
				} while (tabuleiro[novaLinha][novaColuna].peca == 0);
				if (tabuleiro[novaLinha][novaColuna].peca != 0 && tabuleiro[novaLinha][novaColuna].peca != jogador) {
					novaLinha++;
					novaColuna--;
					if (testaLimite(novaLinha, novaColuna) && tabuleiro[novaLinha][novaColuna].peca == 0) {
						jogadas[*linhaAux][*colunaAux] = linhaAtual;
						(*linhaAux)++;
						jogadas[*linhaAux][*colunaAux] = colunaAtual;
						(*colunaAux)++;
						*linhaAux = 0;
						cont = 5;
					} else {
						cont++;
					}
				} else {
					cont++;
				}
				break;
			}
		}
	}
}
// FIM FLUXOGRAMA CHECARCOMERDAMA //

// FLUXOGRAMA COMER //
int comer(int jogador, jogadores *jogador1, jogadores *jogador2, int *comeu, int *linhaAtual, int *colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]) {       // Função responsável por comer peças
	// EXISTEM QUATRO POSSIBILIDADES DE DIAGONAIS PARA COMER //
	int cont, linhaAux, colunaAux, passou = 0;
	if (testaLimite(novaLinha, novaColuna)) {
		if (tabuleiro[*linhaAtual][*colunaAtual].peca != 0 && tabuleiro[*linhaAtual][*colunaAtual].peca == jogador) {
			if (tabuleiro[*linhaAtual][*colunaAtual].dama == 1) {
				// COMER COM DAMA //
				comerDama(jogador, jogador1, jogador2, comeu, linhaAtual, colunaAtual, novaLinha, novaColuna, tabuleiro);
			} else {
				// COMER COM PEÇA NORMAL //
				cont = 1;
				while (cont <= 4) {
					// TESTE PARA VERIFICAR SE COMEU E ATUALIZAR O TABULEIRO //
					if (passou) {
						tabuleiro[*linhaAtual][*colunaAtual].peca = 0;
						tabuleiro[linhaAux][colunaAux].peca = jogador;
						*linhaAtual = linhaAux;
						*colunaAtual = colunaAux;
						if (jogador == 1) {
							(*jogador2).pecas--;
						} else {
							(*jogador1).pecas--;
						}
						checaVirouDama(jogador, tabuleiro, *jogador1, *jogador2, novaLinha, novaColuna);
						*comeu = 1;
						return 1;
					}
					switch(cont) {
							// 1 -> DIAGONAL PARA CIMA/ESQUERDA //
						case 1: {
							linhaAux = *linhaAtual - 1;
							colunaAux = *colunaAtual - 1;
							if (testaLimite(linhaAux, colunaAux)) {
								if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
									linhaAux = linhaAux - 1;
									colunaAux = colunaAux - 1;
									if ((novaLinha == linhaAux) && (novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0)) {
										tabuleiro[*linhaAtual-1][*colunaAtual-1].peca = 0;
										tabuleiro[*linhaAtual-1][*colunaAtual-1].dama = 0;
										passou = 1;
									} else {
										cont++;
									}
								} else {
									cont++;
								}
							} else {
								cont++;
							}
							break;
						}
						// 2 -> DIAGONAL PARA CIMA/DIREITA //
						case 2: {
							linhaAux = *linhaAtual - 1;
							colunaAux = *colunaAtual + 1;
							if (testaLimite(linhaAux, colunaAux)) {
								if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
									linhaAux = linhaAux - 1;
									colunaAux = colunaAux + 1;
									if ((novaLinha == linhaAux) && (novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0)) {
										tabuleiro[*linhaAtual-1][*colunaAtual+1].peca = 0;
										tabuleiro[*linhaAtual-1][*colunaAtual+1].dama = 0;
										passou = 1;
									} else {
										cont++;
									}
								} else {
									cont++;
								}
							} else {
								cont++;
							}
							break;
						}
						// 3 -> DIAGONAL PARA BAIXO/DIREITA //
						case 3: {
							linhaAux = *linhaAtual + 1;
							colunaAux = *colunaAtual + 1;
							if (testaLimite(linhaAux, colunaAux)) {
								if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
									linhaAux = linhaAux + 1;
									colunaAux = colunaAux + 1;
									if ((novaLinha == linhaAux) && (novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0)) {
										tabuleiro[*linhaAtual+1][*colunaAtual+1].peca = 0;
										tabuleiro[*linhaAtual+1][*colunaAtual+1].dama = 0;
										passou = 1;
									} else {
										cont++;
									}
								} else {
									cont++;
								}
							} else {
								cont++;
							}
							break;
						}
						// 1 -> DIAGONAL PARA BAIXO/ESQUERDA //
						case 4: {
							linhaAux = *linhaAtual + 1;
							colunaAux = *colunaAtual - 1;
							if (testaLimite(linhaAux, colunaAux)) {
								if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
									linhaAux = linhaAux + 1;
									colunaAux = colunaAux - 1;
									if ((novaLinha == linhaAux) && (novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0)) {
										tabuleiro[*linhaAtual+1][*colunaAtual-1].peca = 0;
										tabuleiro[*linhaAtual+1][*colunaAtual-1].dama = 0;
										passou = 1;
									} else {
										cont++;
									}
								} else {
									cont++;
								}
							} else {
								cont++;
							}
							break;
						}
					}
				}
				if (comeu) {
					return 0;
				}
				return 0;
			}
		}
	}
}
// FIM FLUXOGRAMA COMER //

// FLUXOGRAMA COMERDAMA //
int comerDama(int jogador, jogadores *jogador1, jogadores *jogador2, int *comeu, int *linhaAtual, int *colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]) {   // Função responsável por comer peças utilizando damas
	// EXISTEM QUATRO POSSIBILIDADES DE DIAGONAIS PARA COMER //
	int linhaAux, colunaAux, passou = 0;
	linhaAux = *linhaAtual;
	colunaAux = *colunaAtual;
	if (*linhaAtual > novaLinha) {
		if (*colunaAtual > novaColuna) {
			// 1 -> DIAGONAL PARA CIMA/ESQUERDA //
			do {
				if (testaLimite(linhaAux, colunaAux)) {
					linhaAux = linhaAux - 1;
					colunaAux = colunaAux - 1;
				} else {
					break;
				}
			} while (tabuleiro[linhaAux][colunaAux].peca == 0);
			if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
				linhaAux = linhaAux - 1;
				colunaAux = colunaAux - 1;
				if (testaLimite(linhaAux, colunaAux)) {
					if (novaLinha == linhaAux && novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0) {
						tabuleiro[linhaAux+1][colunaAux+1].peca = 0;
						tabuleiro[linhaAux+1][colunaAux+1].dama = 0;
						passou = 1;
					}
				}
			}
		} else {
			// 2 -> DIAGONAL PARA CIMA/DIREITA//
			do {
				if (testaLimite(linhaAux, colunaAux)) {
					linhaAux = linhaAux - 1;
					colunaAux = colunaAux + 1;
				} else {
					break;
				}
			} while (tabuleiro[linhaAux][colunaAux].peca == 0);
			if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
				linhaAux = linhaAux - 1;
				colunaAux = colunaAux + 1;
				if (testaLimite(linhaAux, colunaAux)) {
					if (novaLinha == linhaAux && novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0) {
						tabuleiro[linhaAux+1][colunaAux-1].peca = 0;
						tabuleiro[linhaAux+1][colunaAux-1].dama = 0;
						passou = 1;
					}
				}
			}
		}
	} else {
		if (*colunaAtual < novaColuna) {
			// 3 -> DIAGONAL PARA BAIXO/DIREITA //
			do {
				if (testaLimite(linhaAux, colunaAux)) {
					linhaAux = linhaAux + 1;
					colunaAux = colunaAux + 1;
				} else {
					break;
				}
			} while (tabuleiro[linhaAux][colunaAux].peca == 0);
			if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
				linhaAux = linhaAux + 1;
				colunaAux = colunaAux + 1;
				if (testaLimite(linhaAux, colunaAux)) {
					if (novaLinha == linhaAux && novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0) {
						tabuleiro[linhaAux-1][colunaAux-1].peca = 0;
						tabuleiro[linhaAux-1][colunaAux-1].dama = 0;
						passou = 1;
					}
				}
			}
		} else {
			// 4 -> DIAGONAL PARA BAIXO/ESQUERDA//
			do {
				if (testaLimite(linhaAux, colunaAux)) {
					linhaAux = linhaAux + 1;
					colunaAux = colunaAux - 1;
				} else {
					break;
				}
			} while (tabuleiro[linhaAux][colunaAux].peca == 0);
			if (tabuleiro[linhaAux][colunaAux].peca != 0 && tabuleiro[linhaAux][colunaAux].peca != jogador) {
				linhaAux = linhaAux + 1;
				colunaAux = colunaAux - 1;
				if (testaLimite(linhaAux, colunaAux)) {
					if (novaLinha == linhaAux && novaColuna == colunaAux && tabuleiro[linhaAux][colunaAux].peca == 0) {
						tabuleiro[linhaAux-1][colunaAux+1].peca = 0;
						tabuleiro[linhaAux-1][colunaAux+1].dama = 0;
						passou = 1;
					}
				}
			}
		}
	}
	// TESTE PARA VERIFICAR SE COMEU E ATUALIZAR O TABULEIRO //
	if (passou) {
		tabuleiro[*linhaAtual][*colunaAtual].dama = 0;
		tabuleiro[linhaAux][colunaAux].dama = 1;
		tabuleiro[*linhaAtual][*colunaAtual].peca = 0;
		tabuleiro[linhaAux][colunaAux].peca = jogador;
		*linhaAtual = linhaAux;
		*colunaAtual = colunaAux;
		if (jogador == 1) {
			(*jogador2).pecas--;
		} else {
			(*jogador1).pecas--;
		}
		return 1;
	}
}
// FIM FLUXOGRAMA COMERDAMA //

// FLUXOGRAMA ANDAR //
int andar(int jogador, jogadores jogador1, jogadores jogador2, int linhaAtual, int colunaAtual, int novaLinha, int novaColuna, matriz tabuleiro[][tam]) {       // Função responsável por movimentar peças
	if (testaLimite(novaLinha, novaColuna)) {
		if (tabuleiro[linhaAtual][colunaAtual].peca == jogador) {
			if (tabuleiro[linhaAtual][colunaAtual].dama == 1) {
				// ANDAR COM PEÇA DAMA //
				if (testaDiagonal(linhaAtual, colunaAtual, novaLinha, novaColuna, tabuleiro)) {
					tabuleiro[linhaAtual][colunaAtual].peca = 0;
					tabuleiro[novaLinha][novaColuna].peca = jogador;
					tabuleiro[linhaAtual][colunaAtual].dama = 0;
					tabuleiro[novaLinha][novaColuna].dama = 1;
					return 1;
				}
			} else {
				// ANDAR COM PEÇA NORMAL //
				if (jogador == 1) {
					// JOGADOR 1 //
					if ((novaLinha == linhaAtual-1) && ((novaColuna == colunaAtual-1) || (novaColuna == colunaAtual+1))) {
						if (tabuleiro[novaLinha][novaColuna].peca == 0) {
							tabuleiro[linhaAtual][colunaAtual].peca = 0;
							tabuleiro[novaLinha][novaColuna].peca = jogador;
							checaVirouDama(jogador, tabuleiro, jogador1, jogador2, novaLinha, novaColuna);
							return 1;
						} else {
							return 0;
						}
					} else {
						return 0;
					}
				} else {
					// JOGADOR 2 //
					if ((novaLinha == linhaAtual+1) && ((novaColuna == colunaAtual-1) || (novaColuna == colunaAtual+1))) {
						if (tabuleiro[novaLinha][novaColuna].peca == 0) {
							tabuleiro[linhaAtual][colunaAtual].peca = 0;
							tabuleiro[novaLinha][novaColuna].peca = jogador;
							checaVirouDama(jogador, tabuleiro, jogador1, jogador2, novaLinha, novaColuna);
							return 1;
						} else {
							return 0;
						}
					} else {
						return 0;
					}
				}
			}
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}
// FIM FLUXOGRAMA ANDAR //

// FLUXOGRAMA CHECAVIROUDAMA //
void checaVirouDama(int jogador, matriz tabuleiro[][tam], jogadores jogador1, jogadores jogador2, int novaLinha, int novaColuna) {
	if (jogador == 1) {
		if (novaLinha == jogador1.limite) {
			tabuleiro[novaLinha][novaColuna].dama = 1;
		}
	} else {
		if (novaLinha == jogador2.limite) {
			tabuleiro[novaLinha][novaColuna].dama = 1;
		}
	}
}
// FIM FLUXOGRAMA CHECAVIROUDAMA //
