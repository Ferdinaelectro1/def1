#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
/*
Gestion Produits

Ajouter un produit (id, nom, prix, stock).

Lister tous les produits.

Sauvegarder/charger dans un fichier binaire (products.dat).
*/

//Représentation des infos sur un produit du magasin
struct Produit
{
    unsigned int id;
    char name[75];
    unsigned int prix;
    unsigned int stock;
};

//Ensemble des produits du magasin
std::vector<Produit> products;

//Fonctions
void add_product(const Produit &p);
Produit get_product();
void list_product();
std::vector<Produit> loadProducts();
void write_to_file(Produit& p);

int main()
{
    bool run = true;
    products = loadProducts();
    while (run)
    {    
        std::cout << "***** STOCK_GESTURE ******" <<std::endl;
        std::cout << "A - Ajouter un produit "<<std::endl;
        std::cout << "B - Lister les produit "<<std::endl;
        char input; //options entrée par l'utilisateur
        std::cin >> input;
        switch (input)
        {
        case 'A':
            {
                Produit p;
                p = get_product();
                add_product(p);
                write_to_file(p);
            }
            break;
        case 'B':
            list_product();
            break;
        default:
            std::cout << "Options invalide"<<std::endl;
            break;
        }
        char cont_input;
        std::cout << "Voulez vous continuer ?? O - OUI , N - NON ";
        std::cin >> cont_input;
        if(cont_input == 'N')
            run = false;
    }
    return 0;
}

void add_product(const Produit &p)
{
    products.push_back(p);
}

Produit get_product()
{
    Produit p;
    std::cout << "Entrez id : ";
    std::cin >> p.id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');//vide le' '\n' présent dans le buffer de std::cin 
    std::cout << "Entrez name : ";
    std::cin >> p.name;
    std::cout << "Entrez prix : ";
    std::cin >> p.prix;
    std::cout << "Entrez le stock actuel : ";
    std::cin >> p.stock;
    return p;
}

void list_product()
{
    for(int i = 0; i < products.size(); i++)
    {
        std::cout << "Produit => "<<products[i].name <<std::endl;
        std::cout << "           Id => "<< products[i].id<<std::endl;
        std::cout << "           Prix => "<< products[i].prix<<std::endl;
        std::cout << "           Disponible => "<<products[i].stock <<std::endl;
    }
}

void write_to_file(Produit& p)
{
    std::ofstream out("products.dat",std::ios::binary | std::ios::app);
    if(!out)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier products.dat"<<std::endl;
        return;
    }
    out.write(reinterpret_cast<const char *>(&p),sizeof(p));
    out.close();
}

std::vector<Produit> loadProducts()
{
    std::vector<Produit> products;
    Produit current_product_read;
    std::ifstream in("products.dat",std::ios::binary);
    if(!in)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier products.dat" <<std::endl;
        return products;
    }
    size_t size = sizeof(current_product_read);
    std::cout << "passé"<< std::endl;
    while (in.read(reinterpret_cast<char *>(&current_product_read),size))
    {
        products.push_back(current_product_read);
    }
    in.close();
    return products;
}