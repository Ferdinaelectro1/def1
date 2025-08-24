#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <unordered_map>

/*
Gestion Produits

Ajouter un produit (id, nom, prix, stock).

Lister tous les produits.

Sauvegarder/charger dans un fichier binaire (products.dat).

*/
/*
Vente

Saisir un id produit + quantité.

Vérifier stock, décrémenter.

Enregistrer la vente dans un fichier binaire (sales.dat).
*/

//Représentation des infos sur un produit du magasin
struct Produit
{
    char name[75];
    unsigned int prix;
    unsigned int stock;
};

//Ensemble des produits du magasin
std::unordered_map<int,Produit> products;

//Fonctions
void add_product(unsigned int id,const Produit &p);
Produit get_product();
void list_product();
std::unordered_map<int,Produit> loadProducts();
void write_to_file(const int id,Produit& p);
void sell_product();
unsigned int get_id();
void update_history();

int main()
{
    bool run = true;
    products = loadProducts();
    while (run)
    {    
        std::cout << "***** STOCK_GESTURE ******" <<std::endl;
        std::cout << "A - Ajouter un produit "<<std::endl;
        std::cout << "B - Lister les produit "<<std::endl;
        std::cout << "C - Vendre des produits"<<std::endl;
        char input; //options entrée par l'utilisateur
        std::cin >> input;
        switch (input)
        {
        case 'A':
            {
                Produit p;
                unsigned int id = get_id();
                p = get_product();
                add_product(id,p);
                write_to_file(id,p);
            }
            break;
        case 'B':
            list_product();
            break;
        case 'C':
            sell_product();
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

unsigned int get_id()
{
    unsigned int id;
    std::cout << "Entrez l'id du produit : ";
    std::cin >> id;
    return id;
}

void add_product(unsigned int id,const Produit &p)
{
    if(products.find(id) != products.end())
    {
        std::cerr << "Ce id est déjà utilisé"<<std::endl;
        return;
    }
    products[id] = p;
}

Produit get_product()
{
    Produit p;
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
    for(std::unordered_map<int,Produit>::iterator it = products.begin(); it != products.end(); ++it)
    {
        std::cout << "Produit => "<<it->second.name <<std::endl;
        std::cout << "           Id => "<< it->first<<std::endl;
        std::cout << "           Prix => "<< it->second.prix<<std::endl;
        std::cout << "           Stock actuel => "<<it->second.stock <<std::endl;
    }
}

void write_to_file(const int id,Produit& p)
{
    std::ofstream out("products.dat",std::ios::binary | std::ios::app);
    if(!out)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier products.dat"<<std::endl;
        return;
    }
    out.write(reinterpret_cast<const char *>(&id),sizeof(id));
    out.write(reinterpret_cast<const char *>(&p),sizeof(p));
    out.close();
}

std::unordered_map<int,Produit> loadProducts()
{
    std::unordered_map<int,Produit> products;
    int id;
    Produit current_product_read;
    std::ifstream in("products.dat",std::ios::binary);
    if(!in)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier products.dat" <<std::endl;
        return products;
    }
    size_t size = sizeof(current_product_read);
    std::cout << "passé"<< std::endl;
    while (in.read(reinterpret_cast<char *>(&id),sizeof(id)))
    {
        in.read(reinterpret_cast<char *>(&current_product_read),size);
        products[id] = current_product_read;
    }
    in.close();
    return products;
}

void sell_product()
{
    unsigned int id;
    unsigned int qt;
    std::cout << "Entrez l'id du produit : ";
    std::cin >> id;
    if(products.find(id) == products.end())
    {
        std::cerr << "Aucun produit avec cet id !"<<std::endl;
        return;
    }
    std::cout << "Entrez la quantité à vendre : ";
    std::cin >> qt;
    if(qt > products[id].stock)
    {
        std::cerr << "La quantité en stock n'est pas suffisante "<<std::endl;
        return;
    }
    products[id].stock -= qt;
    std::cout << "|Vente de : "<<products[id].name<<" , qantité : "<<qt<<"|"<<std::endl;
    update_history();
}

void update_history()
{
    std::ofstream out("products.dat",std::ios::binary);
    if(!out)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier products.dat"<<std::endl;
        return;
    }
    for(std::unordered_map<int,Produit>::iterator it = products.begin(); it != products.end(); ++it)
    {
        out.write(reinterpret_cast<const char *>(&it->first),sizeof(int));
        out.write(reinterpret_cast<const char *>(&it->second),sizeof(Produit));
    }
    out.close();
}