#include "menu.h"
#include "main.h"
#include "snake.h"
//#define DEV

void initgame (Game *G, Body *B, Apple *A, Wall *W, Settings *S) {

	// Vérification (création si NULL) du fichier 'scores'
	FILE *fichier = NULL;
	fichier = fopen("src/scores", "a");
	fclose(fichier);

  // Attribution VARIABLES DEFAUT
  S->setG.variant = MODERN;
	S->setG.tcase = 14;
	S->setG.width = 60;
	S->setG.height = 40;
	S->setG.score = 0;
	S->setG.level = 0;

	S->setB.nbrseg = 10;
	S->setB.speed = 90000;

	S->setA.eaten = 0;
	S->setA.spawn = 5;

	S->setW.spawn = 10;

	dispMenu(G, B, A, W, S);
}

void dispMenu (Game *G, Body *B, Apple *A, Wall *W, Settings *S) {

	int width = 60 * 14;
	int height = 40 * 14;
	int tcase = 14;
	int touche, old = 1;

	InitialiserGraphique();
	CreerFenetre(500, 300, width, height);
	ChoisirEcran(2);

	#ifdef DEV
		DessinerSegment(width/2, 0, width/2, height);
		DessinerSegment(0, height/2, width, height/2);
	#endif

	while (1) {

		EffacerEcran(CouleurParNom("forestgreen"));
		ChargerImage("src/fonts/snake.png", tcase * 15.5, tcase * 4, 0, 0, 421, 72);
		ChargerImage("src/fonts/play.png", tcase * 25.5, tcase * 14, 0, 0, 128, 52);
		ChargerImage("src/fonts/highscores.png", tcase * 19, tcase * 21, 0, 0, 328, 52);
		ChargerImage("src/fonts/settings.png", tcase * 22, tcase * 28, 0, 0, 225, 52);
		ChargerImage("src/fonts/quit.png", tcase * 26.5, tcase * 35, 0, 0, 91, 42);

		SourisPosition();

		touche = 0;
		if(ToucheEnAttente())
			touche = Touche();

		if(touche == XK_Escape)
			quit();
		if(touche == XK_Down) {
			if(old == 3)
				old = 4;
			if(old == 2)
				old = 3;
			if(old == 1)
				old = 2;
		}
		if(touche == XK_Up) {
			if(old == 2)
				old = 1;
			if(old == 3)
				old = 2;
			if(old == 4)
				old = 3;
		}
		if(touche == XK_Return) {
			if(old == 1)
				return dispPlay(G, B, A, W, *S);
			if(old == 2)
				dispHighscore(G, B, A, W, S);
			if(old == 3)
				dispSettings(G, B, A, W, S);
			if(old == 4)
				quit();
		}

		if(_X >= (tcase * 25.5) && _X <= (tcase * 25.5 + 128) && _Y >= (tcase * 14) && _Y <= (tcase * 14 + 52) || old == 1) {
			ChargerImage("src/fonts/select.png", tcase * 23, tcase * 14.5, 0, 0, 19, 28);
			old = 1;
		}
		if(_X >= (tcase * 19) && _X <= (tcase * 19 + 328) && _Y >= (tcase * 21) && _Y <= (tcase * 21 + 52) || old == 2) {
			ChargerImage("src/fonts/select.png", tcase * 16.5, tcase * 21.5, 0, 0, 19, 28);
			old = 2;
		}
		if(_X >= (tcase * 22) && _X <= (tcase * 22 + 225) && _Y >= (tcase * 28) && _Y <= (tcase * 28 + 52) || old == 3) {
			ChargerImage("src/fonts/select.png", tcase * 19.5, tcase * 28.5, 0, 0, 19, 28);
			old = 3;
		}
		if(_X >= (tcase * 26.5) && _X <= (tcase * 26.5 + 91) && _Y >= (tcase * 35) && _Y <= (tcase * 35 + 42) || old == 4) {
			ChargerImage("src/fonts/select.png", tcase * 24, tcase * 35.5, 0, 0, 19, 28);
			old = 4;
		}

		CopierZone(2, 0, 0, 0, width, height, 0, 0);

		if(SourisCliquee()) {
			if(_X >= (tcase * 25.5) && _X <= (tcase * 25.5 + 128) && _Y >= (tcase * 14) && _Y <= (tcase * 14 + 52))
				return dispPlay(G, B, A, W, *S);
			if(_X >= (tcase * 19) && _X <= (tcase * 19 + 328) && _Y >= (tcase * 21) && _Y <= (tcase * 21 + 52)) {
				dispHighscore(G, B, A, W, S);
				continue;
			}
			if(_X >= (tcase * 22) && _X <= (tcase * 22 + 225) && _Y >= (tcase * 28) && _Y <= (tcase * 28 + 52)) {
				dispSettings(G, B, A, W, S);
				continue;
			}
			if(_X >= (tcase * 26.5) && _X <= (tcase * 26.5 + 91) && _Y >= (tcase * 35) && _Y <= (tcase * 35 + 42))
				quit();
		}
	}
}

void dispPlay (Game *G, Body *B, Apple *A, Wall *W, Settings S) {

	FermerGraphique();

	// Initialisation des paramètres / snake / pommes / murs / création de la fenêtre
	setSettings(G, B, A, W, S);
	body_init(*G, B);
	randomApple(*G, *B, A);
	randomWall(*G, *B, *A, W);
	InitialiserGraphique();
	CreerFenetre(500, 300, G->width * G->tcase, G->height * G->tcase);
}

void dispHighscore (Game *G, Body *B, Apple *A, Wall *W, Settings *S) {

	int width = 60 * 14;
	int height = 40 * 14;
	int touche = 0, temp, temp_l, posy = 120, posx = 320;
	char buf[6], player[11];

	EffacerEcran(CouleurParNom("forestgreen"));
	ChargerImage("src/fonts/highscores_title.png", 14 * 11.75, 14 * 4, 0, 0, 523, 45);
	ChoisirCouleurDessin(CouleurParNom("black"));

	FILE* fichier = NULL;
	fichier = fopen("src/scores", "r");

	int c = 0;
	int line = 0, i = 0, j = 0;

	// On compte les lignes
	while(c != EOF) {
		c = fgetc(fichier);
		if((char)c == '\n')
			line++;
	}

	// La 1re dimension stocke le score, la 2e stocke la ligne
	int (*scores)[2];
	scores = malloc(line * sizeof(*scores));

	// Récupération des scores
	fseek(fichier, 0, SEEK_SET);
	for(i = 0 ; i < line ; i++) {
		scores[i][1] = i;
		fscanf(fichier, "%d", &scores[i][0]);
		while(fgetc(fichier) != '\n');
	}

	// Tri des scores
	for(i = 0; i < line; i++) {
		for(j = i+1; j < line; j++) {
			if(scores[i][0] > scores[j][0]) {
				temp = scores[i][0];
				temp_l = scores[i][1];
				scores[i][0] = scores[j][0];
				scores[i][1] = scores[j][1];
				scores[j][0] = temp;
				scores[j][1] = temp_l;
			}
		}
	}

			#ifdef DEV
	for(i = 0 ; i < line ; i++) {
		printf("score %d: %d\n", scores[i][1], scores[i][0]);
	}
			#endif

	// Récupération et affichage du pseudo | score
	for(i = line-1 ; i >= 0 ; i--) {
		fseek(fichier, 0, SEEK_SET);
		for(temp = 0 ; temp < scores[i][1] ; temp++ ) {
			while(fgetc(fichier) != '\n');
		}
		fscanf(fichier, "%*d %s", player);
		if(player[0] > 97 && player[0] < 122)
			player[0] -= 32;
		player[10] = '\0';
		sprintf(buf, "%d", scores[i][0]);
		if(posy > (height - 42) && posx == 650)
			break;
		if(posy >= height - 42) {
			posy = 120;
			posx = 650;
		}
		posy += 42;
		EcrireTexte(posx - 150, posy, player, 2);
		EcrireTexte(posx, posy, buf, 2);
	}

	EcrireTexte(50, height-18, "Delete", 1);
	ChargerImage("src/redcross.png", 8, height-40, 0, 0, 32, 32);

	CopierZone(2, 0, 0, 0, width, height, 0, 0);

	int verif = 0; 
	while(touche != XK_Escape) {
		if(SourisCliquee() && _X >= 0 && _X <= 37 && _Y >= height-37 && _Y <= height) {
			remove("src/scores");
			fichier = fopen("src/scores", "a");
			break;
		}
		if(ToucheEnAttente())
			touche = Touche();
	}

	fclose(fichier);
	while(SourisCliquee());
	SourisPosition();
	return;
}

void dispSettings (Game *G, Body *B, Apple *A, Wall *W, Settings *S) {

	int width = 60 * 14;
	int height = 40 * 14;
	int touche = 0, adv_settings = 0;
	char buf[5];

	while(touche != XK_Escape) {

		if(ToucheEnAttente())
			touche = Touche();

		EffacerEcran(CouleurParNom("forestgreen"));

		#ifdef DEV
			DessinerSegment(width/2, 0, width/2, height);
			DessinerSegment(0, height/2, width, height/2);
		#endif

		ChargerImage("src/fonts/settings_title.png", 14 * 16.5, 14 * 5, 0, 0, 390, 45);

		ChargerImage("src/fonts/width.png", 220, 200, 0, 0, 69, 18);
		ChargerImage("src/plus.png", 315, 200+32, 0, 0, 32, 32);
		ChargerImage("src/minus.png", 165, 200+32, 0, 0, 32, 32);
		sprintf(buf, "%d", S->setG.width);
		EcrireTexte(240, 250, buf, 2);

		ChargerImage("src/fonts/height.png", 550, 200, 0, 0, 85, 20);
		ChargerImage("src/plus.png", 650, 200+32, 0, 0, 32, 32);
		ChargerImage("src/minus.png", 500, 200+32, 0, 0, 32, 32);

		ChargerImage("src/fonts/startingsize.png", 155, 400, 0, 0, 175, 17);
		ChargerImage("src/plus.png", 320, 400+32, 0, 0, 32, 32);
		ChargerImage("src/minus.png", 130, 400+32, 0, 0, 32, 32);

		ChargerImage("src/fonts/apples.png", 550, 400, 0, 0, 93, 17);
		ChargerImage("src/plus.png", 650, 400+32, 0, 0, 32, 32);
		ChargerImage("src/minus.png", 510, 400+32, 0, 0, 32, 32);

		SourisCliquee();

		if(_X >= 315 && _X <= 315+32 && _Y >= 232 && _Y <= 232+32) {
			printf("Add 2 to width\n");
			S->setG.width += 2;
			if(S->setG.width > 200)
				S->setG.width = 200;
		}
		if(_X >= 165 && _X <= 165+32 && _Y >= 232 && _Y <= 232+32) {
			printf("Remove 2 to width\n");
			S->setG.width -= 2;
			if(S->setG.width < 24)
				S->setG.width = 24;
		}

		_X = 0;
		_Y = 0;
		CopierZone(2, 0, 0, 0, width, height, 0, 0);
	}

	while(SourisCliquee());
	SourisPosition();
	return;
}

void quit () {

	FermerGraphique();
	printf("▁ ▂ ▄ ▅ ▆ ▇ █ Merci d'avoir joué ! █ ▇ ▆ ▅ ▄ ▂ ▁\n");
	exit(0);
}

// On initialise tous les paramètres (pré)définis
void setSettings (Game *G, Body *B, Apple *A, Wall *W, Settings S) {

	*G = S.setG;

	B->nbrseg = S.setB.nbrseg;
	B->speed = S.setB.speed;
	B->s_seg = malloc((B->nbrseg+1) * sizeof(Segment));

	A->eaten = S.setA.eaten;
	A->spawn = S.setA.spawn;
	A->x = malloc(A->spawn * sizeof(int));
	A->y = malloc(A->spawn * sizeof(int));
	A->exist = malloc(A->spawn * sizeof(int));

	W->spawn = S.setW.spawn;
	W->x = malloc(W->spawn * sizeof(int));
	W->y = malloc(W->spawn * sizeof(int));
}

void setScore (Game G) {

	// Si le score est inférieur à 1 | son pseudo est déjà inscrit avec le même score
	if(G.score < 1 || verifScore(getenv("USER"), G.score))
		return;

	FILE* fichier = NULL;
	fichier = fopen("src/scores", "a");

	// Le score et le nom d'utilisateur sont inscrits
	fprintf(fichier, "%d %s\n", G.score, getenv("USER"));

	fclose(fichier);
}

int verifScore (char *pseudo, int score) {

	int c = 0;
	char ps[11];
	int tmp;
	FILE* fichier = NULL;
	fichier = fopen("src/scores", "r+");

	// Si le score et le pseudo correspondent à une entrée, on n'inscrit rien
	while((char)c != EOF) {

		fscanf(fichier, "%d %s", &tmp, ps);
		//printf("score: %d ps: %s\n", tmp, ps);
		if(tmp == score && !strcmp(pseudo, ps))
			return 1;

		fseek(fichier, 1, SEEK_CUR);

		c = fgetc(fichier);
		if(c != EOF)
			fseek(fichier, -1, SEEK_CUR);
	}

	fclose(fichier);
	return 0;
}

// b : Background | d : Dessin | p : Pause | t : timer/score
couleur choisirCouleur (Theme T, char type) {

	static int randr = 0, randv = 0, randb = 0, randr1 = 0, randv1 = 0, randb1 = 0;
	static int random = 35;
	if(random > 35)
		random = 0;

	couleur C;

	if(T == RETRO) {
		if(type == 'b')
			C = CouleurParNom("NULL");
		if(type == 'd')
			C = CouleurParNom("NULL");
		if(type == 'p')
			C = CouleurParNom("NULL");
	}
	if(T == CLASSIC) {
		if(type == 'b')
			C = CouleurParNom("NULL");
		if(type == 'd')
			C = CouleurParNom("NULL");
		if(type == 'p')
			C = CouleurParNom("NULL");
	}
	if(T == MODERN) {
		if(type == 'b')
			C = CouleurParComposante(24, 89, 60);
		if(type == 'd')
			C = CouleurParComposante(200, 145, 0);
		if(type == 'p')
			C = CouleurParComposante(30, 72, 54);
		if(type == 't')
			C = CouleurParComposante(4, 69, 40);
	}
	if(T == RANDOM) {
		if(random == 35)
			randr = rand()%255, randv = rand()%255, randb = rand()%255;
		if(type == 'b')
			C = CouleurParComposante(randr, randv, randb);

		if(random == 35)
			randr1 = rand()%255, randv1 = rand()%255, randb1 = rand()%255;
		if(type == 'd')
			C = CouleurParComposante(randr1, randv1, randb1);

		if(type == 'p')
			C = CouleurParComposante(0, 0, 0);
		random++;
	}

	return C;
}