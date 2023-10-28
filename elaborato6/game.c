#include "game.h"

//tutte le macro sviluppate sono di controllo ritornano true o false (1-0)

#define SHIFT_COL(b,p,n) ((n)==(p.x)-(b.x))  //Controllo se la distanza fra la x della palla e la  x della racchetta è n
#define SAME_ROW(b,p,len) ((((p.y)+(len-1))>=(b.y)) && ((p.y)<=(b.y))) //Controllo se la pallina è nella stessa riga (y) dela alla racchetta   //len -1
#define TOP_ROW(b,p) ((b.y) == ((p.y)-1)) //Controllo se la pallina è una sopra rispetto alla racchetta
#define BOT_ROW(b,p,len) ((b.y) == ((p.y)+len)) //Controllo se la pallina è uno sotto rispetto alla racchetta

//la palla puo toccare il pad in modi differenti in alto(TOP) o in basso (BOT) oppure al centro (FULL)
//(-1)  la pallasi trova 1 a Sinistra rispetto alla racchetta (0) Se si trova sullo stesso punto della racchetta (1) 1 A Destra rispetto alla racchetta
#define PAD1_TOP_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p1,-1) || SHIFT_COL(G.b,G.p1,0) || SHIFT_COL(G.b,G.p1,1)) && TOP_ROW(G.b, G.p1)) 
#define PAD1_BOT_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p1,-1) || SHIFT_COL(G.b,G.p1,0) || SHIFT_COL(G.b,G.p1,1)) && BOT_ROW(G.b, G.p1,G.pad_len))
#define PAD2_TOP_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p2,2) || SHIFT_COL(G.b,G.p2,1) || SHIFT_COL(G.b,G.p2,0)) && TOP_ROW(G.b, G.p2))
#define PAD2_BOT_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p2,2) || SHIFT_COL(G.b,G.p2,1) || SHIFT_COL(G.b,G.p2,0)) && BOT_ROW(G.b, G.p2,G.pad_len))
#define PAD1_FULL_TOUCH(G) (SHIFT_COL(G.b,G.p1,-1) && SAME_ROW(G.b,G.p1,G.pad_len))
#define PAD2_FULL_TOUCH(G) (SHIFT_COL(G.b,G.p2, 2) && SAME_ROW(G.b,G.p2,G.pad_len))
#define PAD_TOP_BORDER_TOUCH(G) (PAD1_TOP_BORDER_TOUCH(G) || PAD2_TOP_BORDER_TOUCH(G))
#define PAD_BOT_BORDER_TOUCH(G) (PAD1_BOT_BORDER_TOUCH(G) || PAD2_BOT_BORDER_TOUCH(G))

//comportamento della palla quando tocca il bodo del campo a destra sinistra sotto o sopra
#define BALL_TOP_BORDER_TOUCH(G) ((G).b.y == 0) 
#define BALL_BOT_BORDER_TOUCH(G)  ((G).b.y == G.height) 
#define BALL_BOT_TOP_BORDER_TOUCH(G) (BALL_BOT_BORDER_TOUCH(G) || BALL_TOP_BORDER_TOUCH(G)) 
#define BALL_LEFT_BORDER_TOUCH(G) ((G).b.x == 0)
#define BALL_RIGHT_BORDER_TOUCH(G) ((G).b.x == (G).width-1)
#define BALL_BORDER_TOUCH(G) (BALL_LEFT_BORDER_TOUCH(G) || BALL_RIGHT_BORDER_TOUCH(G))

struct ball {
	int x; 
	int y; 
	int h; 
	int v;
};

struct paddle {
	int x; 
	int y;
};

struct game {
	unsigned int score1;
	unsigned int score2;
	int pad_len;
	int width;
	int height;
	int starting_ball_x;
	int starting_ball_y;
	struct ball b;
	struct paddle p1;
	struct paddle p2;
};

 static struct game G;     //static per nascondere le altre strutture e renderle visibili a livello di file


void setup_game(int height, int width,                 //funzione per ricevere le informazioni dal gioco per poi modificarle
	struct position ball_pos, struct position ball_dir,
	struct position pad1_pos, struct position pad2_pos, int pad_len) {      
	G.pad_len = pad_len;
	G.p1.x = pad1_pos.x;
	G.p1.y = pad1_pos.y;
	G.p2.x = pad2_pos.x;
	G.p2.y = pad2_pos.y;
	G.height = height;
	G.width = width;
	G.starting_ball_x = ball_pos.x;
	G.starting_ball_y = ball_pos.y;
	G.b.h = ball_dir.x;
	G.b.v = ball_dir.y;
	G.b.x = ball_pos.x;
	G.b.y = ball_pos.y;
	G.score1 = G.score2 = 0;
}


void move_ball() {

	
	if (BALL_BORDER_TOUCH(G))   //se la palla tocca il bordo
	{
		if (BALL_RIGHT_BORDER_TOUCH(G))         //assegna i punti al player1 oppure al player2
		{
			G.score1++;
			
		}
		else if(BALL_LEFT_BORDER_TOUCH(G))
		{
			G.score2++;
		}
		

		G.b.x = G.starting_ball_x;      //resetta la poszione della palla al centro quando ttocca il bordo
		G.b.y = G.starting_ball_y;

		if (BALL_BOT_TOP_BORDER_TOUCH(G))        //controlla se nell angolo in alto o in basso la palla tocca il pad
		{
			G.b.v *= -1;
		}


	}
	else       //senno continua a muoversi toccando i pad
	{
		if (BALL_BOT_TOP_BORDER_TOUCH(G))                //ogni volta che la palla tocca i bordi si inverte la direzione verticale
		{
				G.b.v *= -1;
		
		}
		else if (PAD1_FULL_TOUCH(G) || PAD2_FULL_TOUCH(G))        //ogni volta che la palla tocca il pad  al centro si inverte la direzione orizzontale
		{
			G.b.h *= -1;
		}
		else if (PAD_TOP_BORDER_TOUCH(G) || PAD_BOT_BORDER_TOUCH(G))      //ogni volta che la palla tocca il bordo superiore del pad si inverte si la direzione orizzontale che verticale 
		{
			G.b.h *= -1;
			G.b.v *= -1;
		}
		
		
		G.b.x += G.b.h;     // per far muovere la pallina sommo alla sua posizione corrente la direzione verticale alla x e orizzontale alla y
		G.b.y += G.b.v;
	}

}




void move_pad1_up(void) {    
	if (G.p1.y != 0) {                  
		if (!PAD1_TOP_BORDER_TOUCH(G))  //per evitare che la racchetta e la pallina si sovrappongano e che superino il bordo del campo
		{
			G.p1.y--;
		}
	}
}

void move_pad2_up(void) {
	if (G.p2.y != 0) {
		if (!PAD2_TOP_BORDER_TOUCH(G))  //per evitare che la racchetta e la pallina si sovrappongano e che superino il bordo del campo
		{
			G.p2.y--;
		}
	}
}

void move_pad1_down(void) {
	
	if (G.p1.y != (G.height - G.pad_len + 1)) {
		if (!PAD1_BOT_BORDER_TOUCH(G))       //per evitare che la racchetta e la pallina si sovrappongano e che superino il bordo del campo
		{
			G.p1.y++;
		}
		
	}
}

void move_pad2_down(void) {
	if (G.p2.y != (G.height - G.pad_len + 1)) {
		if (!PAD2_BOT_BORDER_TOUCH(G))            //per evitare che la racchetta e la pallina si sovrappongano e che superino il bordo del campo
		{
			G.p2.y++;
		}
	}
}

unsigned int get_pad1_score(void) {            //serve per aggiornare costantemente gli score dei pad
	return G.score1;
}

unsigned int get_pad2_score(void) {                      
	return G.score2;
}

struct position get_ball_pos(void) {                     //serve per aggiornare costantemente la posizione della palla
	struct position p = { G.b.x,G.b.y };                 
	return p;
}

struct position get_pad1_pos(void) {                    //serve per aggiornare costantemente la posizione dei pad
	struct position p = { G.p1.x,G.p1.y };
	return p;
}

struct position get_pad2_pos(void) {
	struct position p = { G.p2.x,G.p2.y };
	return p;
}

unsigned int get_pad_len(void) {                        //serve per matenere la lunghezza del pad        
	return G.pad_len;
}
