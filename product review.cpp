#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
using namespace std;

class Product{
	public:
		int id;
		string name;
		int numReviews;
		int total;
		int numOfEachRating[5];

	Product(int ProductId, string ProductName){
		id = ProductId;
		name = ProductName;
		numReviews = 0;
		total = 0;
		for (int x = 0; x < 5; x++){
			numOfEachRating[x] = 0;
		}
	}
	double getAvg(){
		return (double)total / numReviews;
	}

	void printInfo(){
		cout << "\nProduct:\t\t'" << name << "'" << endl;
		cout << "Avg rating:\t" << getAvg() << endl;
		cout << "Num reviews:\t" << numReviews << endl;
		for (int x = 1; x <= 5; x++){
			cout << "Num" << x << "s:\t\t" << numOfEachRating[x - 1] << endl;
		}
		cout << endl;
	}

};//end

class Review{
	public:
		int ProductID;
		int costumerID;
		int rating;

		Review(int Product, int costumer, int ProductRating){
			ProductID = Product;
			costumerID = costumer;
			rating = ProductRating;
		}

};
class Costumer{
	public:
		int id;
		int numOfReviews;

		Costumer(int costumerID, int numReviews){
			id = costumerID;
			numOfReviews = numReviews;
		}
		
};

int main(){

	
	vector<Product> Products;

	
	vector<Review> reviews;

	
	vector<Costumer> costumers;

	ifstream file("text.txt");
	string line;
	int x = 0;
	cout << ">> Reading file... ";
	while (getline(file, line)){
		int id = atoi((line.substr(0, line.find(","))).c_str());
		line = line.substr(line.find(",") + 1);
		Products.push_back(Product(id, line));
		x++;
	}
	cout << x << endl; //The count of number of Products read in from the file

	ifstream file2("text2.txt");
	string line2;
	x = 0;
	cout << ">> Reading file2... ";
	while (getline(file2, line2)){
		istringstream strstream(line2);
		string ProductIDstr, costumerIDstr, ratingstr;
		getline(strstream, ProductIDstr, ',');
		getline(strstream, costumerIDstr, ',');
		getline(strstream, ratingstr, ',');
		int ProductID = atoi(ProductIDstr.c_str());
		int costumerID = atoi(costumerIDstr.c_str());
		int ProductRating = atoi(ratingstr.c_str());
		Products[ProductID - 1].total += ProductRating;
		Products[ProductID - 1].numReviews++;
		Products[ProductID - 1].numOfEachRating[ProductRating - 1]++;
		reviews.push_back(Review(ProductID, costumerID, ProductRating));
		x++;
	}
	cout << x << "\n\n" << endl; 
	sort(Products.begin(), Products.end(), [&](Product m1, Product m2){
		if (m1.getAvg() == m2.getAvg())
			return (strcmp(m1.name.c_str(), m2.name.c_str()) < 0);
		else
			return m1.getAvg() > m2.getAvg();
	});

	cout << ">> Top 100 Products <<\n" << endl;
	cout << "Rank\tID\tReviews Avg\tName" << endl;
	for (int i = 1; i <= 100; i++){
		cout << i << ".\t" << Products[i-1].id << "\t" << Products[i-1].numReviews << "\t"
			<< Products[i-1].getAvg() << "\t" << "'" << Products[i-1].name << "'" << endl;
	}
	cout << "\n" << endl;
	
	sort(Products.begin(), Products.end(), [&](Product m1, Product m2){
		return m1.id < m2.id;
	});
	sort(reviews.begin(), reviews.end(), [&](Review r1, Review r2){
		return r1.costumerID <= r2.costumerID;
	});

	int ID = reviews[0].costumerID;
	int total = 0;
	for (auto &x : reviews){
		if (x.costumerID == ID){
			total++;
		}
		else{
			costumers.push_back(Costumer(ID, total));
			ID = x.costumerID;
			total = 1;
		}
	}

	sort(costumers.begin(), costumers.end(), [&](Costumer u1, Costumer u2){
		if (u1.numOfReviews == u2.numOfReviews)
			return u1.id < u2.id;
		else
			return u1.numOfReviews > u2.numOfReviews;
	});

	cout << ">> Top 50 Costumers <<\n" << endl;
	cout << "Rank\tID\tReviews" << endl;
	for (int i = 1; i <= 50; i++){
		cout << i << ".\t" << costumers[i-1].id << "\t" << costumers[i-1].numOfReviews << endl;
	}

	cout << "\n\n>> Product Information <<\n" << endl;
	int input = -1;
	while (input != 0){
		do{
			cout << "Please enter a Product ID [1-250], or 0 to stop: ";
			cin >> input;
			if (input<0 || input>500)
				cout << "**Invalid Product ID! Please try again." << endl;
		} while (input<0 || input>500);
		if (input == 0)
			break;
		Products[input - 1].printInfo();
	}

	
	cout << "\nDONE" << endl;
	return 1;
}
