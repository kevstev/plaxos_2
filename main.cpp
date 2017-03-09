#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<cstring>
using namespace std;

struct Product
{
    string name;
    unsigned price;

};

vector<Product> build_list(char *filename, unsigned cardAmount);


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "incorrect number of arguments" << endl;
        return -1;
    }

    int giftcardAmount = atoi(argv[2]);

    vector < Product > products = build_list(argv[1], giftcardAmount);

    // if no two items were less than giftcard size, exit. 
    if (products.size() < 2)
    {
        cout << "Not possible" << endl;
	return 0;
    }

    /* The basic idea is to iterate from the largest value to the smallest values, finding the
       searching for the smallest leftover amount. If we iterate and do not find a *smaller* 
       leftover amount, we can stop looking through the inner loop as we will not find one. 
       This means our typical case is considerably better than O(n^2)
    */
    
    int smallest_delta = giftcardAmount;
    Product prod1; // keep track of product pair. could be an std::pair
    Product prod2;
    int num_iters = 0; 

    for (int i = products.size() - 1; i >= 0; i--)
    {
        //no need to look at pairs we have already looked at, so set j=i,
        //and no doubles allowed, hence the j=i-1
        for (int j = i - 1; j >= 0; j--) 
        {    
            num_iters++;
            int delta = giftcardAmount - (products[i].price + products[j].price);          
            //cout << products[i].price << "," << products[j].price << "\t delta: " << delta << endl;

            if ((delta < smallest_delta) && (delta >= 0)) //delta < 0 means we overspent
            {
	        //cout << "found a new smallest: delta " << delta << endl;
                prod1 = products[i];
                prod2 = products[j];
                smallest_delta = delta;
            } 	    
	    else if (delta >= 0) //we haven't overspent, but we aren't any closer to an optimal solution either
            {
	      //cout << "no new delta found, continue on!" << endl;
                break;
            }
            
	    if (delta == 0) //minor optimization, you can't do better than spending it all!
	      break;
        }

    }

    //cout << "solution took:" << num_iters << endl;
    cout << prod1.name << prod1.price << "," << prod2.name << " " << prod2.price << endl;
    return 0;
}

vector<Product> build_list(char *filename, unsigned giftcardAmount)
{
    ifstream input_file(filename);

    if (!input_file.is_open())
    {
        cout << "Could not open file\n";
        exit(EXIT_FAILURE);
    }
    string token;
    vector < Product > products;

    while (getline(input_file, token, ','))
    {
        Product product;
        product.name = token;
        getline(input_file, token);

        product.price = stoi(token);

        if (product.price < giftcardAmount)
            products.push_back(product);
    }

    return products;
}
