#include "hash.hpp"

void Swap(Block *a, Block *b)
{
    Item aux;
    aux = a->data;
    a->data = b->data;
    b->data = aux;
}

void EscritaArquivo()
{

    int matrix_size = 0, value = 0;
    ofstream file;

    cout << "\nInforme quantas linhas e colunas vai compor a matriz principal: ";
    cin >> matrix_size;

    file.open("matriz.txt");

    if (!file)
    {
        cout << "Arquivo não pode ser aberto" << endl;
        abort();
    }
    srand(time(NULL));
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            value = (rand() % 100);
            file << value << "\t";
        }
        file << "\n";
    }

    cout << "\nUm arquivo contendo uma matriz " << matrix_size << " x " << matrix_size << " foi criado" << endl;

    file.close();

    ManipulaArquivo(matrix_size);
}

void ManipulaArquivo(int matrix_size)
{

    string line, **matrix;
    int linha1 = 0, coluna1 = 0, linha2 = 0, coluna2 = 0, size = 0, cont = 0, linha = 0, coluna = 0;
    ifstream file;

    file.open("matriz.txt");

    cout << "\nInforme o 'i' do 1º ponto: ";
    cin >> linha1;
    cout << "Informe o 'j' do 1º ponto: ";
    cin >> coluna1;
    cout << "\nInforme o 'i' do 2º ponto: ";
    cin >> linha2;
    cout << "Informe o 'j' do 2º ponto: ";
    cin >> coluna2;

    if (linha1 >= matrix_size || linha2 >= matrix_size)
    {
        cout << "\nERRO: Linha ou coluna inserida não existente!" << endl;
        exit(1);
    }

    // Calculo para descobrir o tamanho da matriz secundaria
    size = ((linha2 - (linha1 - 1)) * (coluna2 - (coluna1 - 1)));

    // Aloca a memoria para inserir a matriz
    matrix = (string **)malloc(sizeof(string *) * size);
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (string *)malloc(sizeof(string) * size);
    }

    for (int i = 0; i < matrix_size; i++)
    {
        getline(file, line);
        if (i >= linha1)
        {
            if (i <= linha2)
            {
                vector<string> tokens;
                istringstream iss(line);
                string number;
                while (iss >> number)
                {
                    tokens.push_back(number);
                }
                cont = 0;
                for (coluna = 0; coluna < (linha2 - linha1); coluna++)
                {
                    if (cont <= (coluna2 - coluna1))
                    {
                        matrix[linha][coluna] = tokens[coluna1 + cont];
                        cont++;
                    }
                }
                linha++;
            }
        }
    }
    file.close();

    linha = (linha2 - linha1);
    coluna = (coluna2 - coluna1);

    PrintMatrix(linha, coluna, matrix);
}

void PrintMatrix(int linha, int coluna, string **matrix)
{
    for (int i = 0; i < coluna; i++)
    {
        cout << "\n";
        for (int j = 0; j < linha; j++)
        {
            cout << matrix[i][j] << "\t";
        }
    }
    cout << "\n\n";
}

void FHVazia(Hash *l)
{
    l->first = (Block *)malloc(sizeof(Block));
    l->last = l->first;
    l->first->prox = NULL;
}

void HInsert(Hash *l, Item d)
{
    l->last->prox = (Block *)malloc(sizeof(Block));
    l->last = l->last->prox;
    l->last->data = d;
    l->last->prox = NULL;
}

void HRemove(Hash *l, Item d)
{
    Block *aux, *tmp;

    if (l->first == l->last || l == NULL || l->first->prox == NULL)
    {
        printf("Hash VAZIA!\n");
        return;
    }

    aux = l->first;
    while (aux->prox != NULL)
    {
        if (aux->prox->data.val == d.val)
        {
            tmp = aux;
            aux = aux->prox;
            tmp->prox = aux->prox;
            if (aux == l->last)
                l->last = tmp;
            free(aux);
            aux->prox = NULL;
        }
        else
            aux = aux->prox;
    }
}

void HImprime(Hash *l, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Chave:%d - Valor: %d\n", l[i].first->data.key, l[i].first->data.val);
    }
}

// Faz com que o tamanho da hash passado dobre e seja o número primo proximo desse dobro
int FoldSize(int vector_size)
{

    int i = 0, cont = 0;

    vector_size = vector_size * 2; // dobra

    // vai transformar em número primo
    while (1)
    {
        for (i = 1; i <= vector_size; i++)
        {
            if (vector_size % i == 0)
            {
                cont++;
            }
        }
        if (cont == 2)
        {
            return vector_size;
        }
        else
        {
            vector_size++;
            cont = 0;
        }
    }
}

void LinearHashing(int vector_size, int hash_size, int *input_vector)
{
    Hash LinearHashing[hash_size];
    Item aux;
    int i = 0;
    int key = 0;

    for (i = 0; i < hash_size; i++)
    {
        FHVazia(&LinearHashing[i]);
    }

    for (i = 0; i < vector_size; i++)
    {
        aux.val = input_vector[i];
        key = KeyCalculate1(aux.val, hash_size); // primeiro calcula a chave com mod, depois verifica se tem valor dentro dela e ai insere
        aux.key = key;
        if (LinearHashing[key].first == LinearHashing[key].last)
        {
            /*verifica se vai haver colisão, se houver vai inserir mais uma posição da hash e somar um no contador de colisão */
            HInsert(&LinearHashing[key], aux);
        }
        else
        {
            HInsert(&LinearHashing[key], aux);
        }
    }
}

// Vai achar a chave atraves da função mod
int KeyCalculate1(int hash_value, int hash_size)
{

    int key = 0;

    key = hash_value % (hash_size);

    return key;
}