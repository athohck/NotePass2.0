#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h> 

//Creation Compte//
void creationCompte(){
    char id[200], password[200];
    int cle = 0;
    printf("Cree ton id : ");
    fgets(id, 200, stdin);
    id[strcspn(id, "\n")] = '\0';   //permet de supprimer le \n et mettre le \o
    printf("Cree ton mot de passe : ");
    fgets(password, 200, stdin);
    password[strcspn(password, "\n")] = '\0';
    //Verification de la taille du mot de passe 
    if (strlen(password) <= 10)
    {
        printf("Ton mot de passe na pas assez de caracteres\n");
        printf("Tu n'a saisie que %zu caracteres, 11 caracteres recquis\n", strlen(password));
    }
    else
    {
        printf("Cree une cle de ciffrement avec des chiffres : ");
        scanf("%d", &cle);
        char cleStr[20];         // une boite pour stocker le nombre en texte
        sprintf(cleStr, "%d", cle);  // transforme cle en texte et le met dans cleStr
        printf("Compte cree avec succee\n");
        //Hash de l'id
        unsigned char hashId[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)id, strlen(id), hashId);
        //Hash du password
        unsigned char hashPassword[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)password, strlen(password), hashPassword);
        //Hash de la cle de chiffrement 
        unsigned char hashCle[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)cleStr, strlen(cleStr), hashCle);
        //Création du fichier de sauvegarde
        FILE *fichier = fopen("Compte.txt", "a");
        //Vérification de l'éxistance du fichier
        if (fichier == NULL)
        {
            printf("ERREUR: Fichier introuvable");
            return;
        }
        //ENregistre le hash de hashId
        
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            fprintf(fichier, "%02x", hashId[i]);
        }
        fprintf(fichier, "\n");
        //Enregistre le hash de hashPaswword
        
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            fprintf(fichier, "%02x", hashPassword[i]);
        }
        fprintf(fichier, "\n");
        //Enregistre le hash de hashCle
        
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            fprintf(fichier, "%02x", hashCle[i]);
        }
        fprintf(fichier, "\n");
        fclose(fichier);
    }
    
    

}
////
//Menu Coffre
void menuCoffre(){
    printf("1: ajouter au coffre\n");
    printf("2: consulter coffre\n");
    printf("3: quitter cofre\n");
}
////
//ajouut au coffre fort//
void ajoutCoffre(char *nomFichier, char *cleHasher){
    //Saisi du compte et mot de passe 
    char nomCompte[60], passwordCompte[60];
    char cleHasherStr[66];
    int cle = 0;
    printf("Entre ta cle de chiffrement : ");
    scanf("%d", &cle);
    getchar();
    //Chiffrement de la cle que l'utilisateur entre pour la comparer avec celle qui la donner quand il a cree son compte 
    sprintf(cleHasherStr, "%d", cle);
    unsigned char hashCle[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)cleHasherStr, strlen(cleHasherStr), hashCle);
    //Convertit le hashCle en string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(cleHasherStr + (i * 2), "%02x", hashCle[i]);

    }
    if (strcmp(cleHasherStr, cleHasher) == 0)
    {
        printf("Entre le nom du compte : ");
        fgets(nomCompte, 60, stdin);
        nomCompte[strcspn(nomCompte, "\n")] = '\0';
        printf("Entre le mot de pass du compte : ");
        fgets(passwordCompte, 60, stdin);
        passwordCompte[strcspn(passwordCompte, "\n")] = '\0';
        //chiffrement Compte//
        char *nomCompteChiffre = malloc ((strlen(nomCompte) + 1) * sizeof(char)); 
        char *passwordChiffre = malloc ((strlen(passwordCompte) + 1) * sizeof(char));
        //Chiffrement de nom de compte 
        for (int i = 0; i < strlen(nomCompte); i++)
        {
            nomCompteChiffre[i] = nomCompte[i] ^cle;
        }
        //Chiffrement de mot de passe compte 
        for (int i = 0; i < strlen(passwordCompte); i++)
        {
            passwordChiffre[i] = passwordCompte[i] ^ cle;
        }
        
        
        //Enregistrement dans le coffre 
        FILE *fichier = fopen(nomFichier, "a");
        if (fichier == NULL)
        {
            printf("Fichier introuvable\n");
            return;
        }
        fprintf(fichier, "%s\n", nomCompteChiffre);
        fprintf(fichier, "%s\n", passwordChiffre);
        printf("Compte ajouter au coffre\n");
        free(nomCompteChiffre);
        free(passwordChiffre);
        fclose(fichier);  
        }
    else
    {
        printf("Erreur: La cle entrer est incorrecte\n");
    }
    
}
////
//Consulter Coffre//
void afficheCoffre(char *nomFichier, char *cleHasher){
    int cle = 0;
    char cleHasherStr[66];
    printf("Entre ta cle de chiffrement : ");
    scanf("%d", &cle);
    getchar();
    //Chiffrement de la cle que l'utilisateur entre pour la comparer avec celle qui la donner quand il a cree son compte 
    sprintf(cleHasherStr, "%d", cle);
    unsigned char hashCle[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)cleHasherStr, strlen(cleHasherStr), hashCle);
    //Convertit le hashCle en string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(cleHasherStr + (i * 2), "%02x", hashCle[i]);

    }
    if (strcmp(cleHasherStr, cleHasher) == 0){

        char ligne[50];
        char ligneDechiffre[50];
        //Lecture du coffre fort 
        FILE *fichier = fopen(nomFichier, "r");
        if (fichier == NULL)
        {
            printf("Le coffre fort est introuvable\n");
            return;
        }
        int compteur = 0;
        while (fgets(ligne, 50, fichier) != NULL)
        {
            ligne[strcspn(ligne, "\n")] = '\0';
            for (int i = 0; i < strlen(ligne); i++)
            {
                ligneDechiffre[i] = ligne[i] ^ cle;
            }
            ligneDechiffre[strlen(ligne)] = '\0';
            printf("%s\n", ligneDechiffre);
            compteur ++;
            if (compteur % 2 == 0)  //Permet de mettre le separateur toute les deux lignes 
            {
                printf("––––––––––––––\n");
            }
        }
        fclose(fichier);
        }
    else
    {
        printf("ERREUR: La cle entrer est incorrecte\n");
    }
    
}
//Connexion Compte
void connexionCompte(){

    //Variable de stockage id et mdp et information Compte
    char idEntrer[100], passwordEntrer[100], informationCompte[400];
    printf("Entre ton id : ");
    fgets(idEntrer, 100, stdin);
    idEntrer[strcspn(idEntrer, "\n")] = '\0';
    printf("Entre ton mot de passe : ");
    fgets(passwordEntrer, 100, stdin);
    passwordEntrer[strcspn(passwordEntrer, "\n")] = '\0';

    //Hash des id et password entrer
    //IdEntrerHasher
    unsigned char idEntrerHasher[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)idEntrer, strlen(idEntrer), idEntrerHasher);
    //PasswordEntrerHasher
    unsigned char passwordEntrerHasher[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)passwordEntrer, strlen(passwordEntrer), passwordEntrerHasher);

    
    FILE *fichier = fopen("Compte.txt", "r");
    //Verification de lexistance du fichier 
    if (fichier == NULL)
    {
        printf("ERREUR: Fichier introuvable\n");
        return;
    }
    //Variable pour stocker les id et password hasher en string 
    char idEntrerHasherStr[66], passwordEntrerHasherStr[66];
    //Variable trouve 
    int trouve = 0;

    //While pour la verification de compte 
    while (fgets(informationCompte, 400, fichier) != NULL)
    {
        informationCompte[strcspn(informationCompte, "\n")] = '\0';
        //Convertit le idEntrerHasher en string
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            sprintf(idEntrerHasherStr + (i * 2), "%02x", idEntrerHasher[i]);
        }
        //Comparer informationCompte avec idEntrerHasherStr
        if (strcmp(informationCompte, idEntrerHasherStr) == 0)
        {
            trouve = 1;//Trouve = 1 si on trouve l'identifiant 
            fgets(informationCompte, 400, fichier);//On lit la ligne suivante
            informationCompte[strcspn(informationCompte, "\n")] = '\0';
            //Convertit le passwordEntrerHasher en string
            for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
            {
                sprintf(passwordEntrerHasherStr + (i * 2), "%02x", passwordEntrerHasher[i]);
            }
            //Comparer informationCompte avec passwordEntrerHasherStr
            //Connexion correcte
            if (strcmp(informationCompte, passwordEntrerHasherStr) == 0)
            {
                char cleHasher[100];    //Varibale pour stocker la cleHasher de l'utilisateur
                fgets(cleHasher, 100, fichier); //Recupére la cleHasher
                cleHasher[strcspn(cleHasher, "\n")] = '\0';
                int choix = 0;
                printf("Connexion Compte Reussi\n");
                system("osascript -e 'display notification \"Connexion a notePass\" with title \"NotePass\"'"); //Permet de cree une notification
                char nomFciher[60]; //Va contenir le nom id pour cree et se connecter au fichier du coffre
                strcpy(nomFciher, idEntrer);    //Copy le idEnter dans nomFichier
                char extentionCompte[] = "_Coffre.txt"; //Ajoute lextention et le format du fichier pour le coffre de l'utilisateur
                strcat(nomFciher, extentionCompte); //Maitenant nomFichier contient idEnter+ExtentonFichier
                do
                {
                    menuCoffre();
                    scanf("%d", &choix); 
                    //Ajouter au coffre 
                    if (choix == 1)
                    {
                        ajoutCoffre(nomFciher, cleHasher);
                        
                    }
                    else if (choix == 2)
                    {
                        afficheCoffre(nomFciher, cleHasher);
                    }
                    else if (choix == 3)
                    {
                        printf("Sort du coffre\n");
                    }
                    else{
                        printf("Chox incorrect\n");
                    } 
                    
                } while (choix != 3);
                
            }
            else
            {
                printf("Mot de passe ou identifiant incorrete\n");

            }
        }
    }
    if (trouve == 0)
    {
        printf("Identifiant introuvale\n");
    }
}

void Menu(){
    printf("1: Creation Compte\n");
    printf("2: Connexion Compte\n");
    printf("3: Quitter\n");
}
int main(){
    int choix = 0;
    
    do
    {
        Menu();
        printf(">");
        scanf("%d", &choix);
        getchar();

        if (choix == 1)
        {
            creationCompte();
        }
        else if (choix == 2)
        {
            connexionCompte();
        }
        else if (choix == 3)
        {
            printf("Au Revoir");
            break;
        }
        
        
    } while (choix != 3);
    
    return 0;
}