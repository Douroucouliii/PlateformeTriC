#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

typedef struct
{
    int nb_comparaisons;
    int nb_permutations;
} Nb;

typedef struct
{
    char *nom_tri;
    int taille_liste;
    Nb *stats;
} Stat;

int *init_list(int taille)
{
    int *tab = malloc(sizeof(int) * taille);
    for (unsigned int i = 0; i < taille; i++)
    {
        int r = rand() % 100;
        *(tab + i) = r;
    }
    return tab;
}

void afficher_tab(int *tab, int taille)
{
    printf("\n[");
    for (unsigned int i = 0; i < taille - 1; i++)
    {
        printf("%d,", tab[i]);
    }
    printf("%d]\n", tab[taille - 1]);
}

void permute(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

unsigned char est_triee(int *tab, int taille)
{
    int i = 0;
    while (i != taille - 1)
    {
        if (tab[i] > tab[i + 1])
        {
            return 0;
        }
        i++;
    }
    return 1;
}

void tri_selection(int *tab, int taille, Nb *nb_selection)
{
    int premier = 0;
    while (premier < taille)
    {
        int min = premier;
        for (unsigned int i = premier; i < taille; i++)
        {
            nb_selection->nb_comparaisons++;
            if (tab[i] < tab[min])
            {
                min = i;
            }
        }
        permute(&tab[premier], &tab[min]);
        nb_selection->nb_permutations++;
        premier++;
    }
}

void tri_insertion(int *tab, int taille, Nb *nb_insertion)
{
    for (unsigned int i = 1; i < taille; i++)
    {
        int ref = tab[i];
        int j = i - 1;
        while (j >= 0 && (nb_insertion->nb_comparaisons++, tab[j] > ref))
        {
            nb_insertion->nb_permutations++;
            permute(&tab[j + 1], &tab[j]);
            tab[j] = ref;
            j = j - 1;
        }
    }
}

void tri_bulle(int *tab, int taille, Nb *nb_bulle)
{
    unsigned char bulle = 1;
    unsigned int i = 0;
    while (i < taille && bulle)
    {
        bulle = 0;
        for (unsigned int j = 0; j < taille - i - 1; j++)
        {
            nb_bulle->nb_comparaisons++;
            if (tab[j] > tab[j + 1])
            {
                nb_bulle->nb_permutations++;
                permute(&tab[j], &tab[j + 1]);
                bulle = 1;
            }
        }
        i += 1;
    }
}

void fusionner(int *tab, int deb, int milieu, int fin, Nb *nb_fusion)
{
    int taille1 = milieu - deb + 1;
    int taille2 = fin - milieu;

    int *gauche = malloc(sizeof(int) * taille1);
    int *droite = malloc(sizeof(int) * taille2);

    for (int i = 0; i < taille1; i++)
    {
        gauche[i] = tab[deb + i];
    }
    for (int j = 0; j < taille2; j++)
    {
        droite[j] = tab[milieu + j + 1];
    }

    int i = 0;
    int j = 0;
    int k = deb;

    while (i < taille1 && j < taille2)
    {
        nb_fusion->nb_comparaisons++;
        if (gauche[i] <= droite[j])
        {
            nb_fusion->nb_permutations++;
            tab[k] = gauche[i];
            i++;
        }
        else
        {
            nb_fusion->nb_permutations++;
            tab[k] = droite[j];
            j++;
        }
        k++;
    }

    while (i < taille1)
    {
        nb_fusion->nb_permutations++;
        tab[k] = gauche[i];
        i++;
        k++;
    }
    while (j < taille2)
    {
        nb_fusion->nb_permutations++;
        tab[k] = droite[j];
        j++;
        k++;
    }

    free(gauche);
    free(droite);
}
void tri_fusion(int *tab, int deb, int fin, Nb *nb_fusion)
{
    if (deb < fin)
    {
        int milieu = (deb + fin) / 2;
        tri_fusion(tab, deb, milieu, nb_fusion);
        tri_fusion(tab, milieu + 1, fin, nb_fusion);
        fusionner(tab, deb, milieu, fin, nb_fusion);
    }
}

int partitionner(int *tab, int deb, int fin, Nb *nb_rapide)
{
    assert(deb < fin - 1);
    int pivot = tab[fin - 1];
    int borne_inf = deb;
    int borne_sup = fin - 1;

    while (borne_inf < borne_sup)
    {
        nb_rapide->nb_comparaisons++;
        if (tab[borne_inf] < pivot)
        {
            borne_inf++;
        }
        else
        {
            nb_rapide->nb_permutations++;
            tab[borne_sup] = tab[borne_inf];
            tab[borne_inf] = tab[borne_sup - 1];
            borne_sup--;
        }
    }
    tab[borne_inf] = pivot;
    return borne_inf;
}
void tri_rapide(int *tab, int deb, int fin, Nb *nb_rapide)
{
    if (deb < fin - 1)
    {
        int pivot = partitionner(tab, deb, fin, nb_rapide);
        tri_rapide(tab, deb, pivot, nb_rapide);
        tri_rapide(tab, pivot + 1, fin, nb_rapide);
    }
}

void tri_tas(int *tab, int taille, Nb *nb_tas)
{
    // Construire un tas max à partir du tableau initial
    for (int i = taille / 2 - 1; i >= 0; i--)
    {
        int ref = i;
        while (2 * ref + 1 < taille)
        {
            int fg = 2 * ref + 1;
            int fd = 2 * ref + 2;
            int max = fg;
            if (fd < taille && (nb_tas->nb_comparaisons++, tab[fd] > tab[fg]))
            {
                max = fd;
            }
            nb_tas->nb_comparaisons++;
            if (tab[max] > tab[ref])
            {
                nb_tas->nb_permutations++;
                permute(&tab[max], &tab[ref]);
                ref = max;
            }
            else
            {
                break;
            }
        }
    }

    // Supprimer le maximum à chaque itération pour trier le tableau
    for (int i = taille - 1; i > 0; i--)
    {
        nb_tas->nb_permutations++;
        permute(&tab[0], &tab[i]);
        int ref = 0;
        while (2 * ref + 1 < i)
        {
            int fg = 2 * ref + 1;
            int fd = 2 * ref + 2;
            int max = fg;

            if (fd < i && (nb_tas->nb_comparaisons++, tab[fd] > tab[fg]))
            {
                max = fd;
            }
            nb_tas->nb_comparaisons++;
            if (tab[max] > tab[ref])
            {
                nb_tas->nb_permutations++;
                permute(&tab[max], &tab[ref]);
                ref = max;
            }
            else
            {
                break;
            }
        }
    }
}

void plateforme(int taille, int nb_listes, int nb_expes)
{
    FILE *comp;
    FILE *perm;
    FILE *comp_perm;
    FILE *compdat;
    FILE *permdat;
    FILE *comp_permdat;
    comp = fopen("comp.csv", "w");
    perm = fopen("perm.csv", "w");
    comp_perm = fopen("comp_perm.csv", "w");
    compdat = fopen("comp.dat", "w");
    permdat = fopen("perm.dat", "w");
    comp_permdat = fopen("comp_perm.dat", "w");

    if (comp == NULL || perm == NULL || comp_perm == NULL || compdat == NULL || permdat == NULL || comp_permdat == NULL)
    {
        printf("Impossible d'ouvrir un des fichiers.\n");
        exit(0);
    }

    // Sortie standard
    printf("\nStatistiques          |        Tri selection        |        Tri insertion        |         Tri bulles          |          Tri fusion         |        Tri rapide           |       Tri tas\n");

    // Sortie csv nombre comparaisons
    fprintf(comp, "Nombre comparaisons,Tri selection,Tri insertion,Tri bulles,Tri fusion,Tri rapide,Tri tas\n");
    // Sortie csv nombre permutations
    fprintf(perm, "Nombre permutations,Tri selection,Tri insertion,Tri bulles,Tri fusion,Tri rapide,Tri tas\n");
    // Sortie csv nombre comparaisons+permutations
    fprintf(comp_perm, "Nombre comp+perm,Tri selection,Tri insertion,Tri bulles,Tri fusion,Tri rapide,Tri tas\n");

    for (int i = 1; i <= nb_listes; i++)
    {

        // Pour chaque tri on alloue de la mémoire pour notre structure, on met le nom et la taille.

        // tri selection
        Stat *selection = (Stat *)malloc(sizeof(Stat));
        selection->nom_tri = "selection";
        selection->taille_liste = taille * i;
        Nb *nb_selection = (Nb *)malloc(sizeof(Nb));
        nb_selection->nb_comparaisons = 0;
        nb_selection->nb_permutations = 0;

        // tri insertion
        Stat *insertion = (Stat *)malloc(sizeof(Stat));
        insertion->nom_tri = "insertion";
        insertion->taille_liste = taille * i;
        Nb *nb_insertion = (Nb *)malloc(sizeof(Nb));
        nb_insertion->nb_comparaisons = 0;
        nb_insertion->nb_permutations = 0;

        // tri bulle
        Stat *bulle = (Stat *)malloc(sizeof(Stat));
        bulle->nom_tri = "bulle";
        bulle->taille_liste = taille * i;
        Nb *nb_bulle = (Nb *)malloc(sizeof(Nb));
        nb_bulle->nb_comparaisons = 0;
        nb_bulle->nb_permutations = 0;

        // tri fusion
        Stat *fusion = (Stat *)malloc(sizeof(Stat));
        fusion->nom_tri = "fusion";
        fusion->taille_liste = taille * i;
        Nb *nb_fusion = (Nb *)malloc(sizeof(Nb));
        nb_fusion->nb_comparaisons = 0;
        nb_fusion->nb_permutations = 0;

        // tri rapide
        Stat *rapide = (Stat *)malloc(sizeof(Stat));
        rapide->nom_tri = "rapide";
        rapide->taille_liste = taille * i;
        Nb *nb_rapide = (Nb *)malloc(sizeof(Nb));
        nb_rapide->nb_comparaisons = 0;
        nb_rapide->nb_permutations = 0;

        // tri tas
        Stat *tas = (Stat *)malloc(sizeof(Stat));
        tas->nom_tri = "tas";
        tas->taille_liste = taille * i;
        Nb *nb_tas = (Nb *)malloc(sizeof(Nb));
        nb_tas->nb_comparaisons = 0;
        nb_tas->nb_permutations = 0;

        for (int j = 0; j < nb_expes; j++)
        {
            /*
            Pour chaque nb d'exemplaire d'une taille donnée, on crée une liste aléatoire, qu'on copie
            pour chaque tri. Puis on les tris en ajoutant les permutations et les comparaisons dans les
            structrures, on vérifie que la liste est bien trié (assert), et on libère l'espace mémoire.
            */
            int *tab = init_list(taille * i);
            int *tab_tri = (int *)malloc(taille * i * sizeof(int));

            memcpy(tab_tri, tab, taille * i * sizeof(int));
            tri_selection(tab_tri, taille * i, nb_selection);
            assert(est_triee(tab_tri, taille * i));

            memcpy(tab_tri, tab, taille * i * sizeof(int));
            tri_insertion(tab_tri, taille * i, nb_insertion);
            assert(est_triee(tab_tri, taille * i));

            memcpy(tab_tri, tab, taille * i * sizeof(int));
            tri_bulle(tab_tri, taille * i, nb_bulle);
            assert(est_triee(tab_tri, taille * i));

            memcpy(tab_tri, tab, taille * i * sizeof(int));
            tri_fusion(tab_tri, 0, taille * i - 1, nb_fusion);
            assert(est_triee(tab_tri, taille * i));

            memcpy(tab_tri, tab, taille * i * sizeof(int));
            tri_rapide(tab_tri, 0, taille * i, nb_rapide);
            assert(est_triee(tab_tri, taille * i));

            memcpy(tab_tri, tab, taille * i * sizeof(int));
            tri_tas(tab_tri, taille * i, nb_tas);
            assert(est_triee(tab_tri, taille * i));

            free(tab);
            free(tab_tri);
        }
        // On finit par faire la moyenne pour avoir nos statistiques.

        nb_selection->nb_comparaisons /= nb_expes;
        nb_selection->nb_permutations /= nb_expes;
        selection->stats = nb_selection;

        nb_insertion->nb_comparaisons /= nb_expes;
        nb_insertion->nb_permutations /= nb_expes;
        insertion->stats = nb_insertion;

        nb_bulle->nb_comparaisons /= nb_expes;
        nb_bulle->nb_permutations /= nb_expes;
        bulle->stats = nb_bulle;

        nb_fusion->nb_comparaisons /= nb_expes;
        nb_fusion->nb_permutations /= nb_expes;
        fusion->stats = nb_fusion;

        nb_rapide->nb_comparaisons /= nb_expes;
        nb_rapide->nb_permutations /= nb_expes;
        rapide->stats = nb_rapide;

        nb_tas->nb_comparaisons /= nb_expes;
        nb_tas->nb_permutations /= nb_expes;
        tas->stats = nb_tas;

        // Sortie standard
        printf("Taille : %-10d   |", taille * i);
        printf("    c:%-10d p:%-10d|", nb_selection->nb_comparaisons, nb_selection->nb_permutations);
        printf("    c:%-10d p:%-10d|", nb_insertion->nb_comparaisons, nb_insertion->nb_permutations);
        printf("    c:%-10d p:%-10d|", nb_bulle->nb_comparaisons, nb_bulle->nb_permutations);
        printf("    c:%-10d p:%-10d|", nb_fusion->nb_comparaisons, nb_fusion->nb_permutations);
        printf("    c:%-10d p:%-10d|", nb_rapide->nb_comparaisons, nb_rapide->nb_permutations);
        printf("    c:%-10d p:%-10d\n", nb_tas->nb_comparaisons, nb_tas->nb_permutations);

        // Sortie csv nombre comparaisons
        fprintf(comp, "Taille %d,%d,%d,%d,%d,%d,%d\n", taille * i, selection->stats->nb_comparaisons, insertion->stats->nb_comparaisons, bulle->stats->nb_comparaisons, fusion->stats->nb_comparaisons, rapide->stats->nb_comparaisons, tas->stats->nb_comparaisons);
        // Sortie csv nombre permutations
        fprintf(perm, "Taille %d,%d,%d,%d,%d,%d,%d\n", taille * i, selection->stats->nb_permutations, insertion->stats->nb_permutations, bulle->stats->nb_permutations, fusion->stats->nb_permutations, rapide->stats->nb_permutations, tas->stats->nb_permutations);
        // Sortie csv nombre comparaisons+permutations
        fprintf(comp_perm, "Taille %d,%d,%d,%d,%d,%d,%d\n", taille * i, selection->stats->nb_permutations + selection->stats->nb_comparaisons, insertion->stats->nb_permutations + insertion->stats->nb_comparaisons, bulle->stats->nb_permutations + bulle->stats->nb_comparaisons, fusion->stats->nb_permutations + fusion->stats->nb_comparaisons, rapide->stats->nb_permutations + rapide->stats->nb_comparaisons, tas->stats->nb_permutations + tas->stats->nb_comparaisons);

        // Sortie dat nombre comparaisons
        fprintf(compdat, "%d %d %d %d %d %d %d\n", taille * i, selection->stats->nb_comparaisons, insertion->stats->nb_comparaisons, bulle->stats->nb_comparaisons, fusion->stats->nb_comparaisons, rapide->stats->nb_comparaisons, tas->stats->nb_comparaisons);
        // Sortie dat nombre permutations
        fprintf(permdat, "%d %d %d %d %d %d %d\n", taille * i, selection->stats->nb_permutations, insertion->stats->nb_permutations, bulle->stats->nb_permutations, fusion->stats->nb_permutations, rapide->stats->nb_permutations, tas->stats->nb_permutations);
        // Sortie dat nombre comparaisons+permutations
        fprintf(comp_permdat, "%d %d %d %d %d %d %d\n", taille * i, selection->stats->nb_permutations + selection->stats->nb_comparaisons, insertion->stats->nb_permutations + insertion->stats->nb_comparaisons, bulle->stats->nb_permutations + bulle->stats->nb_comparaisons, fusion->stats->nb_permutations + fusion->stats->nb_comparaisons, rapide->stats->nb_permutations + rapide->stats->nb_comparaisons, tas->stats->nb_permutations + tas->stats->nb_comparaisons);
    }

    fclose(comp);
    fclose(perm);
    fclose(comp_perm);
    fclose(compdat);
    fclose(permdat);
    fclose(comp_permdat);
}

int main(void)
{
    srand(time(NULL));

    plateforme(10, 10, 1000);

    // int *tab = init_list(20);
    // afficher_tab(tab, 20);
    // Nb *nb = (Nb *)malloc(sizeof(Nb));
    // tri_tas(tab, 20, nb);
    // afficher_tab(tab, 20);

    return 0;
}