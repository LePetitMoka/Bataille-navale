//
//  main.cpp
//  Bataille-Navale
//
//  Created by Sebastien Mokemat on 06/10/2022.
//

#include <iostream>// pour cout...
#include <cstdlib> //pour nombre aleatoire
#include <ctime>   //pour nombre aleatoire
#include <iomanip> //pour filtrer les choix
#include <limits.h>//couplé à iomanip pour filtrer les choix
#include <fstream> //pour ecriture fichier

// Boost

//#include </Users/smokemat/Documents/Librairies_C++/boost_1_81_0/boost/serialization/access.hpp>
//#include </Users/smokemat/Documents/Librairies_C++/boost_1_81_0/boost/archive/text_oarchive.hpp>
//#include </Users/smokemat/Documents/Librairies_C++/boost_1_81_0/boost/archive/text_iarchive.hpp>

using namespace std;
//using namespace boost;
//using namespace boost::archive;


//========== Variables globales ==============//

const int T = 8; // taille de la grille

//---------- Grilles de vérification ---------//

const char grilleModeleVerifBT2 [8][8]= {{'A','E','E','E','E','E','E','B'},
                                         {'F','T','T','T','T','T','T','H'},
                                         {'F','T','T','T','T','T','T','H'},
                                         {'F','T','T','T','T','T','T','H'},
                                         {'F','T','T','T','T','T','T','H'},
                                         {'F','T','T','T','T','T','T','H'},
                                         {'F','T','T','T','T','T','T','H'},
                                         {'C','G','G','G','G','G','G','D'}};

const char grilleModeleVerifBT3 [8][8]= {{'A','A','E','E','E','E','B','B'},
                                         {'A','E','E','E','E','E','B','B'},
                                         {'F','F','T','T','T','T','H','H'},
                                         {'F','F','T','T','T','T','H','H'},
                                         {'F','F','T','T','T','T','H','H'},
                                         {'F','F','T','T','T','T','H','H'},
                                         {'C','C','G','G','G','G','D','D'},
                                         {'C','C','G','G','G','G','D','D'}};

const char grilleModeleVerifBT4 [8][8]= {{'A','A','A','E','E','B','B','B'},
                                         {'A','A','A','E','E','B','B','B'},
                                         {'A','A','A','E','E','B','B','B'},
                                         {'F','F','F','T','T','H','H','H'},
                                         {'F','F','F','T','T','H','H','H'},
                                         {'C','C','C','G','G','D','D','D'},
                                         {'C','C','C','G','G','D','D','D'},
                                         {'C','C','C','G','G','D','D','D'}};

int grilleZero[8][8]=      {{0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0}};

//================== Fonctions globales ======================//

template <typename T>

void getChoice(T& choice, string commentaire = "Choix:"){
    
    cin >> setw(1) >> choice;
    
    while(!cin.good()){
        
        cout << "Erreur, reessayez" << endl;
        cout << commentaire << endl;
        
        //nettoie le courant
        cin.clear();
        cin.ignore(INT_MAX,'\n');
        
        cout << commentaire ;
        cin >> setw(1) >> choice;
        
    }
    //nettoie le courant
    cin.clear();
    cin.ignore(INT_MAX,'\n');

}

void clearScreen(){
    cout << string( 100, '\n' );
}

int randomXY(int n = T){  // Generateur de nombre sur l'intervalle [1;T]
    int x;
    srand(time(0));
    x = (rand() % n) - 1;
    return x;
}

char randomDir(){
    int x = 0;
    x = randomXY(4);
    char list[4] ={'H','B','G','D'};
    char c = list[x];
    return c;
}

bool verifDirVar(char c){
    bool b = true;
    if (c != 'H' && c != 'B' && c != 'G' && c != 'D'){
        b = false;
    }
    return b;
}

int LetterToNumber(char letter){ // Prend une lettre en char et renvoie sa position dans l'alphabet en int
    //char Alpha[8]={'A','B','C','D','E','F','G','H'};
    int number;
    letter = toupper(letter);
    /*for (int i=0;i<8;i++){
        if(letter == Alpha[i]){
            number = i + 1;
        }
    }*/
    switch (letter){
        case 'A': number = 1;break;
        case 'B': number = 2;break;
        case 'C': number = 3;break;
        case 'D': number = 4;break;
        case 'E': number = 5;break;
        case 'F': number = 6;break;
        case 'G': number = 7;break;
        case 'H': number = 8;break;
        default: number = T+1;break;
    }
    return number;
}
bool verifNombre(int x){ //Vérifie que le nombre soit entre 1 et la taille maximale de la grille (bornes incluses)
    bool cond = true;
    if (x < 0 || x > T-1){
        cond = false;
    }
    return cond;
}

bool conversionCharGrilleVerif(char cg, char c){ // cg = grilleModeleVerifBTN[x][y]; c = direction. Convertis les characteres des grilles de verif en conditions et renvoie vrai ou faux selon la direction c en parametre. Permet de dire dans quelle direction un bateau peut etre orienté selon son point d'origine.
    bool b = true;
    switch(cg){
        case 'A': switch(c){
            case 'H': b = false;break;
            case 'B':break;
            case 'G': b = false;break;
            case 'D':break;
        } break;
        case 'B': switch(c){
            case 'H': b = false;break;
            case 'B':break;
            case 'G':break;
            case 'D': b = false;break;
        } break;
        case 'C': switch(c){
            case 'H':break;
            case 'B': b = false;break;
            case 'G': b = false;break;
            case 'D':break;
        } break;
        case 'D': switch(c){
            case 'H':break;
            case 'B':b = false;break;
            case 'G':break;
            case 'D':b = false;break;
        } break;
        case 'E': switch(c){
            case 'H': b = false;break;
            case 'B':break;
            case 'G':break;
            case 'D':break;
        } break;
        case 'F': switch(c){
            case 'H':break;
            case 'B':break;
            case 'G': b = false;break;
            case 'D':break;
        } break;
        case 'G': switch(c){
            case 'H':break;
            case 'B': b = false;break;
            case 'G':break;
            case 'D':break;
        } break;
        case 'H': switch(c){
            case 'H':break;
            case 'B':break;
            case 'G':break;
            case 'D': b = false;break;
        } break;
        default:break;
    }
    return b;
}

//============================ Classes ===============================//

class Vue{
    //friend class boost::serialization::access;
    //    template<class Archive>
    //    void serialize(Archive & ar, const unsigned int version) {
    //        ar & _type;
    //        ar & _cases;
    //    }
    //attributs
    public:
    char _type; //type de grille ('B' pour Basse = affiche les tirs adverses et la flotte, 'H' pour Haute = affiche les )
    char _cases[T][T];
    //constructeur
    Vue(char p){
        _type = p;
        for (int x =0;x<T;x++){
            for(int y = 0 ; y < T; y++){
                _cases[x][y]='.';
            }
        }
    }
    //methodes
    char convertisseur(int S){ // converti la somme de GF[x][y] et GT[x][y] en un caractere
        char c;
        switch(S){
            case 0: c = '.';break; // 0 = aucun tir et aucun bateau
            case 1: if (_type == 'H'){
                        c = '.';
                    } else
                        {c = 'b';
                        } break; // 1 = aucun tir et un bateau (grille basse affiche le bateau, grille haute n'affiche pas)
            case 5: c = '*';break; // 5 = un tir et aucun bateau
            case 7: c = 't';break; // 7 = un tir et un bateau touché
            case 8: c = 'd';break; // 8 = un tir et un bateau détruit
            default: c = 'E';break; // erreur
        }
        return c;
    }
    void assemblerVue(int GF[T][T], int GT[T][T]){ // combine les grilles de tir et de flotte: Grille basse = grille flotte J1 + grille tirs J2; Grille haute = grille flotte J2 + grille tirs J1 . Faire la MAJ de la flotte avant de l'assembler !
        for(int x = 0; x < T; x++){
            for(int y = 0; y < T; y++){
                //cout << GF[x][y] << "+" << GT[x][y] << endl;
                _cases[x][y] = convertisseur(GF[x][y]+GT[x][y]);
            }
        }
    }
    void afficherVue(int a = (T+1), int b = (T+1)){ // parametres optionnels (uniquement pour la procedure de placement)
        char Alpha[8]={'A','B','C','D','E','F','G','H'};
        cout << "  1 2 3 4 5 6 7 8" << endl;
        for(int x = 0; x < T; x++){
            cout << Alpha[x] << ' ';
            for(int y = 0; y < T; y++){
                if( x == a && y == b){
                    cout << "o ";
                }else {
                    cout << _cases[x][y] <<' ';
                }
            }
        cout << endl;
        }
    }
};

class Tir{
    //friend class boost::serialization::access;
    //    template<class Archive>
    //    void serialize(Archive & ar, const unsigned int version) {
    //        ar & _nombre;
    //        ar & _grilleTir;
    //    }
    //instanciation
    public:
    int _nombre;
    int _grilleTir[T][T];
    Tir(){
        _nombre = T*T; //à modifier plus tard si l'on veut parametrer le nombre de tir
        for (int x =0;x<T;x++){
            for(int y = 0 ; y < T; y++){
                _grilleTir[x][y]=0;
            }
        }
    }
    //methodes
    bool placerTir(int x, int y){
        bool b = false;
        if (_grilleTir[x][y] == 0){ // verifie si tir disponible
            _grilleTir[x][y] = 5;
            _nombre --;
            b = true;
            
        }
        return b;
    }
};

class Bateau{
    //friend class boost::serialization::access;
    //    template<class Archive>
    //    void serialize(Archive & ar, const unsigned int version) {
    //        ar & _coordonnees;
    //        ar & _etat;
    //        ar & _taille;
    //    }
    //instanciation
    public:
    int _coordonnees[T][T];
    bool _etat = true; // coulé ou non
    int _taille; // taille du bateau
    //constructeur
    Bateau(int t){
        _taille = t;
        for (int x =0;x<T;x++){
            for(int y = 0 ; y < T; y++){
                _coordonnees[x][y]=0;
            }
        }
    }
    //méthodes
    bool verifierEtat(int x, int y){ //prend les coordonnées du dernier tir effectué par le joueur adverse et le compare avec les coordonnées du bateau
        bool b = true;
        if(_coordonnees[x][y] == 1){
            _coordonnees[x][y] = 2;  // si touché, change l'etat de la cellule du bateau à 2
            b = false;                  // renvoie bool false pour indiquer de dire Touché
        }
        return b;
    }
    bool verifierEtatGen(char h = true){ // verifie l'état général du bateau
        bool b = true;
        if(_etat) {      // empeche de re-vérifier si le bateau est déjà détruit
            int sum = 0, x , y;
            for(x = 0; x < T ; x++){
                for(y = 0 ; y < T ; y++){
                    sum += _coordonnees[x][y];
                }
            }
            if (sum == _taille * 2){                   // si le bateau est détruit, alors:
                for(x = 0; x < T ; x++){
                    for(y = 0 ; y < T ; y++){
                        if( _coordonnees[x][y] == 2){
                            _coordonnees[x][y] = 3;    //change la valeur des cases du bateau à 3
                            _etat = false;              //bateau detruit
                            b = false;                  //renvoie bool false pour indiquer de dire Coulé
                        }
                    }
                }
            }
        }
        return b;
    }
    void placerBateau(int x, int y, char dir){ //place le bateau dans son integralité dans la direction donnée
        for (int i = 0; i < _taille; i++){
            switch (dir) {
                case 'H': _coordonnees[x - i][y] = 1;break;
                case 'B': _coordonnees[x + i][y] = 1;break;
                case 'G': _coordonnees[x][y - i] = 1;break;
                case 'D': _coordonnees[x][y + i] = 1;break;
            }
        }
    }
};

class Flotte{
    //friend class boost::serialization::access;
    //template<class Archive>
    //void serialize(Archive & ar, const unsigned int version) {
    //ar & _coordonnees;
    //ar & _etat;
    //ar & _taille;
    //}
    //instanciation
    public:
    int _coordonnees[T][T]; //coordonnées des bateaux de la flotte
    bool _etat = true; // etat de la flotte (false si tous les bateaux sont détruits)
    int _taille = 3; // nombre de bateaux composants la flotte (note: parametrable à l'avenir ?)
    Bateau BateauF[3]={2,3,4}; // liste de trois bateaux, un de taille 2, un de taille 3, une de taille 4 (note : vecteur à l'avenir ?)
    Flotte(){
        for (int x =0;x<T;x++){
            for(int y = 0 ; y < T; y++){
                _coordonnees[x][y]=0;
            }
        }
    }
    //methodes
    void MAJ(){ //met à jour les coordonnées de la flotte (avec l'etat des cellules des bateaux avec 1 = ok, 2 = endommagé, ou 3 = bateau detruit)
        for (int i = 0 ; i < _taille ; i++){
            for (int x = 0; x < T; x++){
                for (int y = 0; y < T; y++){
                    if(BateauF[i]._coordonnees[x][y] != 0 ){ // exclut les cases vides
                        _coordonnees[x][y] = BateauF[i]._coordonnees[x][y];
                    }
                }
            }
        }
    }
    
    void verifierEtat(int x,int y, char h = true){ //verifie l'etat des bateaux après un tir adverse donné
        bool b = false;
        for (int i = 0; i < _taille; i++){
            if(!BateauF[i].verifierEtat(x, y) && h){
                cout << "Touché !\n";
                b = true;
            }
            if(!BateauF[i].verifierEtatGen() && h){
                cout << "Coulé !\n";
            }
        }
        if (!b && h){        //si n'a pas touché, alors raté
            cout << "Raté !" << endl;
        }
        MAJ();
    }
    void verifierEtatGen(){ //verifie si la flotte est detruite

        int C = _taille;
        for (int i = 0; i < _taille; i++){
            if(BateauF[i]._etat == false){
                C--;
            }
        }
        if(C == 0){
            _etat = false;
        }

    }
    bool verifierPlacement(int x,int y){ //verifie si l'emplacement est libre
        MAJ();
        bool b = true;
        if (_coordonnees[x][y] != 0 ){
            b = false;
        }
        return b;
    }
    bool verifierDirection(char dir, int x , int y, int taille, char h = true){ //verifie que toute les cases dans la direction donnée sont libres et que la longueur du bateau ne depasse pas la grille
        bool b = true;
        switch (taille){
            case 2: b =  conversionCharGrilleVerif(grilleModeleVerifBT2[x][y], dir);break;
            case 3: b =  conversionCharGrilleVerif(grilleModeleVerifBT3[x][y], dir);break;
            case 4: b =  conversionCharGrilleVerif(grilleModeleVerifBT4[x][y], dir);break;
            default: b = false;
        }
        if (!b){
            if (h){ // affiche si humain
                cout << "Le bateau ne doit pas sortir de la grille, réessayez" << endl;
            }
            return b;
        }
        for (int i = 0; i < taille; i++){                          //verifie si bateau dans la direction
            switch (dir) {
                case 'H': if(_coordonnees[x - i][y] == 1){ b = false; };break;
                case 'B': if(_coordonnees[x + i][y] == 1){ b = false; };break;
                case 'G': if(_coordonnees[x][y - i] == 1){ b = false; };break;
                case 'D': if(_coordonnees[x][y + i] == 1){ b = false; };break;
            }
        }
        if (!b && h){ // ,,
                cout << "Il y a un bateau dans cette direction !" << endl;
        }
        return b;
    }
    void placerFlotte(bool h){ //procedure de placement de la flotte
        int x, y;
        char c, dir;
        int compteur = 0;
        Vue VuePlacement = 'B';
        for(int i = 0; i < _taille; i++){
            if (h){                         // procédure HUMAIN
                do{
                    cout << "Saisir point d'origine du bateau de taille " << BateauF[i]._taille << endl;
                    cout << "Ligne : ";
                    getChoice(c,"Ligne:");
                    cout << endl;
                    cout << "Colonne: ";
                    getChoice(y,"Colonne:");
                    cout << endl;
                    x = LetterToNumber(c);
                    x--;
                    y--;
                    cout << x << "  -  " << y << endl;
                    if (!verifNombre(x) || !verifNombre(y)){
                        cout << "Réessayez" << endl;
                    } else if (verifierPlacement(x, y) == false){
                        cout << " Il y a déjà un bateau sur ce point d'origine !" << endl;
                        VuePlacement.assemblerVue(_coordonnees,grilleZero);
                        VuePlacement.afficherVue();
                    } else {
                        VuePlacement.afficherVue(x,y);      //affiche vue basse
                        cout << "Saisir orientation du bateau (H = haut, B = bas, G = gauche, D = droite):";
                        getChoice(dir,"Orientation (H/B/G/D)");
                        dir = toupper(dir);
                        cout << endl;
                        if(verifierDirection(dir, x, y, BateauF[i]._taille)){
                            BateauF[i].placerBateau(x, y, dir);     //insere bateau
                            compteur++;
                            MAJ();
                            VuePlacement.assemblerVue(_coordonnees,grilleZero);
                            VuePlacement.afficherVue();
                        }else {
                            VuePlacement.assemblerVue(_coordonnees,grilleZero);
                            VuePlacement.afficherVue();
                        }
                    }
                }while(compteur != i+1);
            }else{                              //procédure ORDI
                do{
                    x = randomXY();
                    y = randomXY();
                    if (!verifierPlacement(x, y)){
                    } else {
                        dir = randomDir();
                        if(verifierDirection(dir, x, y, BateauF[i]._taille, h)){
                            BateauF[i].placerBateau(x, y, dir);  //insere bateau
                            compteur++;
                            MAJ();
                        }
                    }
                }while(compteur != i+1);
            }
        }
    }
};

class Joueur{
    //friend class boost::serialization::access;
    //  template<class Archive>
    //  void serialize(Archive & ar, const unsigned int version) {
    //      ar & Tirs;
    //      ar & VueH;
    //      ar & VueB;
    //      ar & Tour;
    //      ar & _humain;
    //  }
    //instanciation
public:
    Tir Tirs; //
    Vue VueH = 'H';
    Vue VueB = 'B';
    int Tour = 0;
    bool _humain = true; // humain ou IA ?
    Flotte FlotteJ; // liste de trois bateaux, un de taille 2, un de taille 3, une de taille 4
    //methodes
};
class Partie{
    //friend class boost::serialization::access;
    //    template<class Archive>
    //    void serialize(Archive & ar, const unsigned int version) {
    //      ar & JoueurP;
    //      ar & VueP;
    //      ar & _tour;
    //    }
    //instanciation
    public:
    Joueur JoueurP[2];
    Vue VueP[2] = {'H','B'};
    int _tour = 0;
    Partie(int nbJH){
            if(nbJH == 1){
                JoueurP[0]._humain=true;
                JoueurP[1]._humain=false;
            }else{
                JoueurP[0]._humain=true;
                JoueurP[1]._humain=true;
            }
    }
    //methodes

    bool TourP(){
        bool vainqueur = false;
        char save;
        do{
            _tour ++ ;
            cout << "=========== TOUR " << _tour << " ============\n" << endl;
            for (int J1 = 0, J2 = 1; J1 < 2 && JoueurP[J1].FlotteJ._etat && JoueurP[J2].FlotteJ._etat; J1 ++, J2--){
                if(JoueurP[J1]._humain){
                    cout << "Joueur " << J1 + J1 + J2 << " ? (press ENTER)" << endl;    // Debut du tour Joueur
                    cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
                    cout << "============ JOUEUR " << J1+J1+J2<< " ============\n"  << endl;
                    JoueurP[J1].VueH.assemblerVue(JoueurP[J2].FlotteJ._coordonnees, JoueurP[J1].Tirs._grilleTir);
                    JoueurP[J1].VueB.assemblerVue(JoueurP[J1].FlotteJ._coordonnees, JoueurP[J2].Tirs._grilleTir);
                    cout << "+------= Grille de visée =------+\n" << endl;
                    JoueurP[J1].VueH.afficherVue();
                    cout << "+------= État de votre flotte =------+\n" << endl;
                    JoueurP[J1].VueB.afficherVue();
                }
                Tirer(JoueurP[J1]._humain, J1, J2);
                JoueurP[J2].FlotteJ.verifierEtatGen();
                if (!JoueurP[J2].FlotteJ._etat){
                    vainqueur = true;
                }
                if (JoueurP[J1]._humain){
                    cout << "Fin du tour (press ENTER to continue / press 'q' to quit)";   // Fin du tour Joueur
                    //cin >> save;
                    //if(save == 'q'){
                    //    cout << "Partie enregistree" << endl;
                    //    return 1;
                    //} else {
                    //    cin.clear();
                    //    cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
                    //}
                    cin.clear();
                    cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
                }
                clearScreen();
                
            }
        }while(!vainqueur);
        if (!JoueurP[0].FlotteJ._etat){
            cout << "Le vainqueur est J2 !" << endl;
        } else if (!JoueurP[1].FlotteJ._etat){
            cout << "Le vainqueur est J1 !" << endl;
        } else {
            cout << "Erreur" << endl;
            cout << vainqueur << endl;
        }
        return 0;
    }
    bool Placement(){
        
        for (int i =0; i<2 ; i++){
            if(JoueurP[i]._humain){
                cout << "++++++ Placement des bateaux J" << i+1 << " ++++++" << endl;
                cout << endl;
                cout << "Joueur " << i+1 << " pret ? (press ENTER)" << endl;
                cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
                VueP[1].afficherVue();
            }
            JoueurP[i].FlotteJ.placerFlotte(JoueurP[i]._humain);
            clearScreen();
        }
        cout << "La partie est sur le point de commencer, etes-vous pret ? (Press ENTER)" << endl;
        cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
        if(TourP()){
            return 1;
        } else return 0;
    }
    void Tirer(bool h, int i, int j)  //procédure de tir
    {
        char c;
        int x, y;
        bool b = false;
        if (h){           // HUMAIN
            do{
                cout << "Entrez coordonnées du tir (ligne/colonne)" << endl;
                cout << "Ligne : ";
                getChoice(c,"Ligne:");
                cout << endl;
                cout << "Colonne: ";
                getChoice(y,"Colonne:");
                x = LetterToNumber(c);
                x--;
                y--;
                if(verifNombre(x) && verifNombre(y)){
                    if (!JoueurP[i].Tirs.placerTir(x, y)){
                        cout << "Tir déjà effectué sur cette case" << endl;
                    }else{
                        b = true ;
                        JoueurP[j].FlotteJ.verifierEtat(x, y);
                    }
                }else{ cout << " Position incorrecte, réessayez" << endl;}
            }while(!b);
        }else{                    //ORDI
            do{
                x = randomXY();
                y = randomXY();
                if (!JoueurP[i].Tirs.placerTir(x, y)){
                }else{
                    b = true ;
                    JoueurP[j].FlotteJ.verifierEtat(x, y, h);
                }
            }while(!b);
        }
    }
};

class Jouer{
    public : static void Menu(){
        int choix = 0;
        do{
            cout << "============ MENU BATAILLE NAVALE ============" << endl;
            cout << endl;
            cout << "1 - Lancer nouvelle partie Joueur VS Joueur" << endl;
            cout << "2 - Lancer nouvelle partie Joueur VS Ordi" << endl;
            cout << "3 - Reprendre partie"<< endl;
            cout << "4 - Règles du jeu" << endl;
            cout << "0 - Quitter" << endl;
            cout << endl;
            cout << "Votre choix: ";
            cin >> choix;
            switch(choix){
                case 1:{
                    Partie PartieJcJ = 2;
                    bool result = PartieJcJ.Placement();
                    //if (result){
                    //    std::ofstream ofs("Partie.txt");
                    //    text_oarchive oa(ofs);
                    //    oa << PartieJcJ;
                    //    ofs.close();
                    //}
                    break;
                }
                case 2:{
                    Partie PartieJcO = 1;
                    bool result = PartieJcO.Placement();
                    //if (result){
                    //    std::ofstream ofs("Partie.txt");
                    //    text_oarchive oa(ofs);
                    //    oa << PartieJcO;
                    //    ofs.close();
                    //}
                    break;
                }
                case 3: //Reprendre
                    //std::ofstream ofs("Partie.txt");
                    //boost::archive::text_iarchive ia(ifs);
                    //ia >> Partie;
                    //ofs.close();
                    
                    break;
                case 4: Regles();
                    cout << endl;
                    cout << "___ REGLES ___" << endl;
                    cout << endl;
                    cout << "Au début du jeu, chaque joueur place à sa guise tous les bateaux sur sa grille de façon stratégique. Le but étant de compliquer au maximum la tache de son adversaire, c’est-à-dire détruire tous vos navires. Bien entendu, le joueur ne voit pas la grille de son adversaire. Une fois tous les bateaux en jeu, la partie peut commencer." << endl;
                    cout << "Un à un, les joueurs se tirent dessus pour détruire les navires ennemis." << endl;
                    cout << "Un tir par joueur et par tour" << endl;
                    cout << "Celui qui detruit tous les bateaux est declaré vainqueur" << endl;

                    cout << endl;
                    break;
            }
        }while (choix != 0);
    }
    public : static void Regles(){
        cout << "à faire" << endl;
    }
};

int main() {
    
    //Jouer Jeu;
    //Jeu.Menu();
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.clear();
    Jouer Jeu1;
    Jeu1.Menu();
    
}
