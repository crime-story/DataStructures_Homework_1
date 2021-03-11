#include <iostream>
#include<cstdio>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

double durata, start;
inline double getTime()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (double)(tv.tv_sec * 1000000LL + tv.tv_usec)/1000.0;
}

unsigned long v[100000000], b[100000000], fr[100000000];
int i, n, j, ok, nrtest, nrmaxi, k, maxi, r;

void BubbleSort()
{
    for(i=0;i<n-1;i++) // numarul pasi = numar - 1, deci avem n-1 pasi intotdeauna
    {
        ok = 0; // initializam un ok cu 0, presupunem ca vectorul este sortat
        for(j=0;j<n-i-1;j++) // avem n-i-1 comparatii pentru fiecare pas, folosind aceasta regula putem
            // sa evitam comparatiile care nu sunt necesare cum ar fi acelea cand array-ul este deja sortat
            // dar programul inca executa pasi de comparatie
        {
            if(v[j] > v[j+1]) // daca elementul de pe pozitia j este mai mare decat cel de pe pozitia urmatoare
            {
                swap(v[j],v[j+1]); // le interschibam, i.e. folosim functia swap
                ok = 1; // daca am ajuns aici, inseamna ca am facut o interschimbare, deci vectorul inca nu este sortat
            }
        }
        if(ok == 0) // daca vectorul este sortat pentru a ca nu a mai facut nicio interschimbare
            break;  // am terminat programul si iesim fortat
    }
}

void CountingSort()
{
    memset(fr,0,sizeof(fr)); // se reseteaza vectorul de frecventa fr
    for(i=0;i<n;i++) // initializez un vector de frecventa
    {
        ++fr[v[i]]; // iterez pentru fiecare element numarul aparatiilor sale, crescand frecventa
    }
    for(i=1;i<=maxi;i++) //  de la al doilea element pana la maximul din vectorul de sortat
    {
        fr[i] += fr[i-1]; // actualizam vectorul de frecventa fr, incepand de la i = 1 pana la maxi
        // adunand valoarea de pe pozitia anterioara la cea care urmeaza
        // de ex daca in vectorul intial de frecventa aveam fr[1] = 3 si fr[0] = 5
        // fr[1] = fr[1] + fr[0] = 3 + 5 = 7
    }
    for(i=n-1;i>=0;i--) // plecam de la coada pentru a mentinte stabilitatea algoritmului
    {
        b[--fr[v[i]]] = v[i];  // decrementam cu 1 numarul de aparitii ale fiecarui element
        // si retinem in vectorul b pe acea pozitie elementul v[i]
    }
    for(i=0;i<n;i++)
        v[i]=b[i]; // creez un vector de copiere pentru b
}

void interclas(int p,int poz,int u)///apel functie de interclasare
{
    int b[100000];
    int i=p;///retin prima pozitie a primei jumatati
    int j=poz+1;///retin prima pozitie a celei de a 2 a jumatati
    int k=0;///index vector sortat
    while(i<=poz && j<=u)///cat timp nu s-a terminat de parcurs nici o jumatate
        if (v[i]<v[j])///daca un element din prima jumatate<un element din a 2a jumatate
            b[k++]=v[i++];///il retin intr-un alt vector
        else///daca un element din a 2 a jumatate<un element din prima jumatate
            b[k++]=v[j++];///il retin intr-un alt vector

    while (i<=poz)///daca au mai ramas elemente in prima jumatate
        b[k++]=v[i++];///le adaug in acelasi vector

    while (j<=u)///daca au mai ramas elemente in a doua jumatate
        b[k++]=v[j++];///le adaug in acelasi vector


    int t=p;
    for (k=0; k<(u-p)+1; k++)
        v[t++]=b[k];///vectorul initial devine vectorul sortat
}

void divimp(int p,int u)///apel functie divide et impera
{
    if (p<u)
    {
        int poz=(p+u)/2;///retin pozitia de mijloc a vectorului
        divimp(p,poz);///apelez de la inceput pana la mijloc
        divimp(poz+1,u);///apelez de la mijloc pana la sfarsit
        interclas(p,poz,u);///ordoneaza  cele 2 jumatati
    }
}

void MergeSort()
{
    divimp(0,n-1);
}

int partitie1(int p, int u)
{
    int i,j,aux;
    int piv = v[u];

    i = p - 1;

    for (j = p; j <=u- 1; j++)
    {
        if (v[j] <= piv)
        {
            i++;
            aux=v[i];
            v[i]=v[j];
            v[j]=aux;
        }
    }
    i++;
    aux=v[i];
    v[i]=v[u];
    v[u]=aux;
    return i;
}

int partitie2(int p, int u)
{
    int i,j,aux;
    ///--------------------------

    int mij = p + (u - p) / 2;
    if (v[p] > v[mij])
        swap(v[p], v[mij]);
    if (v[p] > v[u])
        swap(v[p], v[u]);
    if (v[mij] > v[u])
        swap(v[mij], v[u]);

    int piv = v[mij];

    while (1)///while(p<u)
    {
        while (v[p] < piv)++p;
        while (v[u] > piv)--u;
        if (p >= u)
            return u;

        swap(v[p], v[u]);
        p++;
        u--;
    }
}

void quickSort1(int p, int u)
{
    if (p < u)
    {
        int poz = partitie1(p, u);
        quickSort1(p, poz - 1);
        quickSort1(poz + 1,u);
    }
}

void quickSort2(int p, int u)
{
    if (p < u)
    {
        int poz = partitie2(p, u);
        if(poz-p+1<u-poz)
        {
            quickSort2(p, poz);
            quickSort2(poz + 1,u);
        }
        else
        {
            quickSort2(poz+1, u);
            quickSort2(p,poz);
        }
    }
}

void radixSort1(int r, int baza, unsigned long v[], int n)
{
    int fr[baza]; // declar vectorul de frecventa a cifrelor
    // care are dimensiunea maxima numarul bazei

    memset(fr,0,sizeof(fr)); // initializez in memorie
    // un vector de frecventa a cifrelor

    for (int i = 0; i < n; i++) // parcurg vectorul
    {
        int cifra_curenta = (v[i] / r) % baza; // extrag cifra curenta din vector
        fr[cifra_curenta+1]++; // cresc frecventa de aparitie a cifrei curente
    }

    for (int i = 1; i < baza; i++)
    {
        fr[i] = fr[i]+ fr[i-1]; // actualizam vectorul de frecventa fr, incepand de la i = 1 pana la baza
        // adunand valoarea de pe pozitia anterioara la cea care urmeaza
        // de ex daca in vectorul intial de frecventa aveam fr[1] = 3 si fr[0] = 5
        // fr[1] = fr[1] + fr[0] = 3 + 5 = 7
        // aceste sume partiale le facem pentru a afla la ce indice trebuie sa plasez elementele cu o anumita cifra
        // pastrand ordinea relativa
    }

    for (int i = 0; i < n; i++) // parcurg vectorul
    {
        int cifra_curenta = (v[i] / r) % baza; // extrag cifra curenta din vector
        b[fr[cifra_curenta]] = v[i]; // plasez intr-un vector auxiliar b elementul v[i] pe pozitia fr[cifra_curenta]
        // pe care am stabilit-o mai sus
        fr[cifra_curenta]++; // incrementez indicele pentru a stabili pozitia pe care va fi plasat urmatorul element
    }

    for (int i = 0; i < n; i++)
        v[i] = b[i]; // copiez vectorul auxiliar in vectorul initial
}

void RadixSort_baze()
{
    int baza;
    cout << "\nIntroduceti baza in care doriti sa faceti sortarea=";
    cin >> baza;
    int r = 1; // r = baza la puterea i, unde i ia valori de la 0
    // la cate cifre are numarul
    // i reprezinta rangul cifrei in numar
    // unde 0 este rangul cifrei unitatilor
    // 1 este rangul cifrei zecilor
    // 2 este rangul cifrei sutelor
    // etc.

    while (r <= maxi) // cat timp baza la putera i este mai mica
        // decat maximul din vectorul de sortat
    {
        radixSort1(r, baza, v, n); // la primul apel ordonam crescator
        // dupa cifra unitatilor
        // La al doilea apel, ordonam crescator dupa cifra zecilor
        // etc.
        r *= baza; // marim rangul cifrei dupa care facem sortarea
    }
}

int main()
{
    int T;
    //cout << "Introduceti numarul de teste pentru BubbleSort: ";
    //cout << "Introduceti numarul de teste pentru CountSort: ";
    //cout << "Introduceti numarul de teste pentru MergeSort: ";
    //cout << "Introduceti numarul de teste pentru QuickSort: ";
    cout << "Introduceti numarul de teste pentru RadixSort: ";
    cin >> T;
    nrtest = 0; // initializam numarul testului cu 0
    while(nrtest<T) // cat timp numarul nrtest este mai mic decat numarul de teste
    {
        cout << "Numarul de elemente din array: ";
        cin >> n; // citim numarul de teste
        cout << "Introduceti maximul din array: ";
        cin >> nrmaxi; // citim elementul maxim din array
        start = getTime();
        nrtest += 1; // avansam in teste
        cout << "\nTestul " << nrtest << endl << "\n";
        for(i=0;i<n;i++)
            v[i] = rand()%nrmaxi;
        cout << "Array-ul pentru Testul " << nrtest <<" este:\n";
        for(i=0;i<n;i++)
            cout << v[i] << " ";

        for(i=0;i<n;i++)
        {
            if(v[i]>maxi)
                maxi = v[i]; // maximul la counting sort
        }
        //BubbleSort();
        //CountingSort();
        //MergeSort();
        RadixSort_baze();

        //quickSort1(0,n-1);
        //quickSort2(0,n-1);

        cout <<"\nArray-ul sortat:\n";

        for(i=0;i<n;i++)
            cout << v[i] << " ";

        durata = getTime() - start;

        cout<<"\nTimpul de executie al programului in milisecunde " << durata;
    }
    getchar();
    return 0;
}