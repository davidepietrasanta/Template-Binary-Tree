#include "Btree.h"
#include <iostream>
#include <cassert>
#include <string>
//using namespace std;

/**
	Classe custom per il test della classe
	Btree.

	@brief Classe custom
*/
class Classe_custom{
    
private:
    int numero;
	char lettera;
public:
	Classe_custom() {
		this->numero = 0;
		this->lettera = 'a';
	}
    
	Classe_custom(int n, char l) {
		this->numero = n;
		this->lettera = l;
	}
    
	~Classe_custom() {}

	int get_numero() const{
		return numero;
	}

	char get_lettera() const{
		return lettera;
	}

	bool operator==(const Classe_custom& other) const {
		bool n = ( numero == other.get_numero() );
		bool l = ( lettera == other.get_lettera() );
		return ( l && n ) ;
	}

	bool operator>(const Classe_custom& other) const {
		if( numero > other.get_numero() ){
			return true;
		}

		if( numero == other.get_numero() && 
			lettera > other.get_lettera() ){
			return true;
		}
		return false;
	}

	bool operator<(const Classe_custom& other) const {
		if( numero < other.get_numero() ){
			return true;
		}

		if( numero == other.get_numero() && 
			lettera < other.get_lettera() ){
			return true;
		}
		return false;
	}

    
};

std::ostream & operator<<(std::ostream &out, const Classe_custom& t) {
	out << "(";
	out << t.get_numero();
	out << ",";
	out << t.get_lettera();
	out << ")";
	return out;
}



//#define DEBUGGING

void test_Node(){
	std::cout << "TEST Node" << std::endl;
	// test costruttori

	Btree<int>::Node n0; //test Node()
	assert( n0.value == 0 );
	assert( n0.right == nullptr );
	assert( n0.left == nullptr );
	assert( n0.next == nullptr );

	Btree<int>::Node n1s(10); //test Node(T v)
	assert( n1s.value == 10 );
	assert( n1s.right == nullptr );
	assert( n1s.left == nullptr );
	assert( n1s.next == nullptr );

	Btree<int>::Node n2(n1s); //test Node(const Node& n)
	assert( n2.value == 10 );
	assert( n2.get_value() == 10 ); //test get_value()
	assert( n2.right == nullptr );
	assert( n2.left == nullptr );
	assert( n2.next == nullptr );

	n2.value = 4;
	assert( n2.value == 4 );
	assert( n2.get_value() == 4 );
	assert( n1s.value == 10 ); 

	Btree<int>::Node *n1 = new Btree<int>::Node(10);
	n1->insert_node(5);
	assert( n1->get_value() == 10 );
	assert( n1->left->get_value() == 5);
	n1->next = n1->left;
	n1->insert_node(15);
	assert( n1->right->get_value() == 15);
	n1->left->next = n1->right;


	Btree<int>::Node n3(n1);

	assert( n3.value == 10 );
	assert( n3.right->value == 15 );
	assert( n3.left->value == 5 );
	n3.right->value = 20;
	assert( n3.value == 10 );
	assert( n3.right->value == 20 );
	assert( n3.left->value == 5 );
	assert( n1->get_value() == 10 );
	assert( n1->left->get_value() == 5 );
	n1->right->value = 35;
	assert( n3.value == 10 );
	assert( n3.left->value == 5 );
	assert( n1->size() == 3);
	assert( n3.size() == 3);
	
	#ifdef DEBUGGING
	std::cout<<"n3: ";
	n3.stampa();
	std::cout<< std::endl;
	std::cout<<"n3: ";
	n3.stampa();
	std::cout<< std::endl;
	#endif

	typename Btree<int>::Node *arrayL[n3.size()];
	n3.array_node(arrayL, n3.size() ); //test array_node(Node**, unsigned int)
	#ifdef DEBUGGING
	std::cout<<"n3: ";
	for( int i = 0; i < n3.size(); i++){
		std::cout << arrayL[i]->value <<"|";
	}
	std::cout<< "  array_node" << std::endl;
	#endif


	Btree<int>::Node *l8 = new Btree<int>::Node(8);
	l8->insert_node(5);
	l8->next = l8->left;
	l8->insert_node(16);
	l8->next->next = l8->right;
	l8->insert_node(26);
	l8->next->next->next = l8->right->right;
	l8->insert_node(1);
	l8->next->next->next->next = l8->left->left;
	l8->insert_node(3);
	l8->next->next->next->next->next = l8->left->left->right;
	l8->insert_node(6);
	l8->next->next->next->next->next->next = l8->left->right;


	assert( l8->last_next().get_value() == 6 ); 

	typename Btree<int>::Node *arrayL8[l8->size()];
	l8->array_node(arrayL8, l8->size() );
	#ifdef DEBUGGING
	std::cout << "l8: ";
	for( int i = 0; i < l8->size(); i++){
		std::cout << arrayL8[i]->value <<"|";
	}
	std::cout<< "  array_node" << std::endl;
	#endif


	Btree<int>::Node nodeCopyL8(l8);;
	assert( nodeCopyL8.value == 8 );
	assert( nodeCopyL8.next->next->value == 26 );
	assert( nodeCopyL8.next->next->next->value == 1 );
	assert( nodeCopyL8.last_next().get_value() == 6 ); 

	n1->destroy();
	l8->destroy();

}

void test_costruttor_Btree(){
	std::cout << "TEST COSTRUTTORI Btree" << std::endl;

	Btree<int> t1; //test Btree()
	assert(t1.size() ==  0);

	bool error_root = false;
	try{
		t1.get_root(); //test get_root()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_root = true;
	}
	assert ( error_root );

	bool error_last = false;
	try{
		t1.get_last(); //test get_last()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_last = true;
	}
	assert ( error_last );

	Btree<int> t2(10); //test Btree(const T &v)
	assert( t2.size() == 1 );
	assert( t2.get_root() == 10 ); //test get_root
	assert( t2.get_last() == 10 ); //test get_last
	assert( t2.max() == 10 );
	assert( t2.min() == 10 );

	Btree<int>::Node *n1 = new Btree<int>::Node(10); //test insert_node
	n1->insert_node(5);
	n1->insert_node(15);
	n1->insert_node(20);

	assert( n1->size() == 4 ); 

	Btree<int> t4(n1); //test Btree(Node n) 
	assert( t4.size() == 4 );
	assert( t4.get_root() == 10 );
	assert( t4.get_last() == 20 );
	assert( t4.max() == 20 );
	assert( t4.min() == 5 );

	n1->left->value = 4;
	assert( n1->value == 10 );
	assert( n1->left->value == 4 );
	assert( n1->right->value == 15 );
	assert( n1->right->right->value == 20 );
	assert( t4.size() == 4 );
	assert( t4.get_root() == 10 );
	assert( t4.get_last() == 20 );
	assert( t4.max() == 20 );
	assert( t4.min() == 5 );

	Btree<int> t3(t4); //test Btree(const Btree<T,F> &other) 
	assert( t3.size() == 4 ); 
	assert( t3.get_root() == 10 );
	assert( t3.get_last() == 20 );
	assert( t3.max() == 20 );
	assert( t3.min() == 5 );


	Btree<int> t5(t1);
	assert( t5.size() == 0 ); 

	error_root = false;
	try{
		t5.get_root(); //test get_root()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_root = true;
	}
	assert ( error_root );

	error_last = false;
	try{
		t5.get_last(); //test get_last()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_last = true;
	}
	assert ( error_last );


	Btree<int> t6(t2); //test Btree(const Btree<T,F> &other)
	assert( t6.size() == 1 );
	assert( t6.get_root() == 10 ); //test get_root
	assert( t6.get_last() == 10 ); //test get_last
	assert( t6.max() == 10 );
	assert( t6.min() == 10 );


	Btree<int> t7 = t6; //test operator=(const Btree<T,F> &other)
	assert( t7.size() == 1 );
	assert( t7.get_root() == 10 ); //test get_root
	assert( t7.get_last() == 10 ); //test get_last
	assert( t7.max() == 10 );
	assert( t7.min() == 10 );


	Btree<int> t8(10);
	t8 = t4;
	assert( t8.size() == 4 );
	assert( t8.get_root() == 10 );
	assert( t8.get_last() == 20 );
	assert( t8.max() == 20 );
	assert( t8.min() == 5 );

	Btree<int> t9;
	assert( t9.size() == 0 ); 

	t8 = t9;
	
	assert( t8.size() == 0 ); 

	error_root = false;
	try{
		t8.get_root(); //test get_root()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_root = true;
	}
	assert ( error_root );

	error_last = false;
	try{
		t8.get_last(); //test get_last()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_last = true;
	}
	assert ( error_last );


	n1->destroy(n1);
}

void test_my_functor(){
	std::cout << "TEST FUNTORI" << std::endl;

	decrescente<int> d;
	assert( d(9,3) == 1 );
	assert( d(4,7) == -1 );
	assert( d(10,10) == 0);


	crescente<int> c;
	assert( c(9,3) == -1 );
	assert( c(4,7) == 1 );
	assert( c(10,10) == 0);

	pari<int> p;
	assert( p(8) == true );
	assert( p(3) == false );
	assert( p(-8) == true );
	assert( p(-3) == false );

	dispari<int> di;
	assert( di(8) == false );
	assert( di(3) == true );
	assert( di(-8) == false );
	assert( di(-3) == true );


}

void test_insert(){
	std::cout << "TEST INSERT" << std::endl;

	Btree<int> r1 (4);
	assert( r1.size() == 1 );
	assert( r1.get_root() == 4 );
	assert( r1.get_last() == 4 );
	assert( r1.max() == 4 );
	assert( r1.min() == 4 );
	r1.insert(3);
	assert( r1.size() == 2 );
	assert( r1.get_root() == 4 );
	assert( r1.get_last() == 3 );
	assert( r1.max() == 4 );
	assert( r1.min() == 3 );
	r1.insert(5);
	assert( r1.size() == 3 );
	assert( r1.get_root() == 4 );
	assert( r1.get_last() == 5 );
	assert( r1.max() == 5 );
	assert( r1.min() == 3 );
	r1.insert(3); //doppio
	assert( r1.size() == 3 );
	assert( r1.get_root() == 4 );
	assert( r1.get_last() == 5 );
	assert( r1.max() == 5 );
	assert( r1.min() == 3 );
	r1.insert(1); 
	assert( r1.size() == 4 );
	assert( r1.get_root() == 4 );
	assert( r1.get_last() == 1 );
	assert( r1.max() == 5 );
	assert( r1.min() == 1 );


	Btree<int, decrescente<int> > r2 (8);
	assert( r2.size() == 1 );
	assert( r2.get_root() == 8 );
	assert( r2.get_last() == 8 );
	assert( r2.max() == 8 );
	assert( r2.min() == 8 );
	r2.insert(3);
	assert( r2.size() == 2 );
	assert( r2.get_root() == 8 );
	assert( r2.get_last() == 3 );
	assert( r2.max() == 3 );
	assert( r2.min() == 8 );
	r2.insert(5);
	assert( r2.size() == 3 );
	assert( r2.get_root() == 8 );
	assert( r2.get_last() == 5 );
	assert( r2.max() == 3 );
	assert( r2.min() == 8 );
	r2.insert(5); //doppia
	assert( r2.size() == 3 );
	assert( r2.get_root() == 8 );
	assert( r2.get_last() == 5 );
	assert( r2.max() == 3 );
	assert( r2.min() == 8 );
	r2.insert(-12);
	assert( r2.size() == 4 );
	assert( r2.get_root() == 8 );
	assert( r2.get_last() == -12 );
	assert( r2.max() == -12 );
	assert( r2.min() == 8 );


	Btree<int> r3 (0);
	for(int i = 1; i < 100; i++){
		r3.insert(i);
	}
	assert( r3.size() == 100 );
	assert( r3.get_root() == 0 );
	assert( r3.get_last() == 99 );
	assert( r3.max() == 99 );
	assert( r3.min() == 0 );
	

	Btree<int> r4;
	assert( r4.size() ==  0);

	bool error_root = false;
	try{
		r4.get_root();
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_root = true;
	}
	assert ( error_root );

	bool error_last = false;
	try{
		r4.get_last();
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_last = true;
	}
	assert ( error_last );

	r4.insert(4);
	assert( r4.size() == 1 );
	assert( r4.get_root() == 4 );
	assert( r4.get_last() == 4 );
	assert( r4.max() == 4 );
	assert( r4.min() == 4 );
	r4.insert(10);
	assert( r4.size() == 2 );
	assert( r4.get_root() == 4 );
	assert( r4.get_last() == 10 );
	assert( r4.max() == 10 );
	assert( r4.min() == 4 );
	r4.insert(3);
	assert( r4.size() == 3 );
	assert( r4.get_root() == 4 );
	assert( r4.get_last() == 3 );
	assert( r4.max() == 10 );
	assert( r4.min() == 3 );
	r4.insert(5);
	assert( r4.size() == 4 );
	assert( r4.get_root() == 4 );
	assert( r4.get_last() == 5 );
	assert( r4.max() == 10 );
	assert( r4.min() == 3 );
	r4.insert(12);
	assert( r4.size() == 5 );
	assert( r4.get_root() == 4 );
	assert( r4.get_last() == 12 );
	assert( r4.max() == 12 );
	assert( r4.min() == 3 );
	
	
	Btree<int> r5 = r4.subtree( r4.get_root() );
	assert( r5.size() == 5 );
	assert( r5.get_root() == 4 );
	assert( r5.get_last() == 12 );
	assert( r5.max() == 12 );
	assert( r5.min() == 3 );
	r5.insert(2);
	r5.insert(11);
	assert( r5.size() == 7 );
	assert( r5.get_root() == 4 );
	assert( r5.get_last() == 11 );
	assert( r5.max() == 12 );
	assert( r5.min() == 2 );

	
	Btree<int> r6 = r4.subtree( 200 ); //dunque un albero vuoto
	assert( r6.size() == 0 );
	r6.insert(2);
	assert( r6.size() == 1 );
	assert( r6.get_root() == 2 );
	assert( r6.get_last() == 2 );
	assert( r6.max() == 2 );
	assert( r6.min() == 2 );
	r6.insert(11);
	assert( r6.size() == 2 );
	assert( r6.get_root() == 2 );
	assert( r6.get_last() == 11 );
	assert( r6.max() == 11 );
	assert( r6.min() == 2 );

}

void test_search(){
	std::cout << "TEST SEARCH" <<std::endl;

	Btree<int, crescente<int> > i1 (8);
	i1.insert(5);
	i1.insert(1);
	i1.insert(6);
	i1.insert(16);
	i1.insert(3);
	
	assert ( i1.search(6) != 0 ) ;
	assert ( i1.search(10) == 0 );
	assert ( i1.search(8) != 0 ) ;
	assert ( i1.max() == 16 );
	assert ( i1.min() == 1 );
	assert ( i1.has(16) );
	assert ( i1.has(8) );
	assert ( i1.has(5) );
	assert ( i1.has(1) );
	assert ( i1.has(3) );
	assert ( i1.has(-14) ==  false );
	assert ( i1.has(0) ==  false );
	assert ( i1.has(140) ==  false );


	#ifdef DEBUGGING
	std::cout << i1 <<std::endl;
	#endif


	Btree<int, decrescente<int> > i2 (8);
	i2.insert(5);
	i2.insert(1);
	i2.insert(6);
	i2.insert(6);
	i2.insert(3);
	
	assert ( i2.search(3) != 0 );
	assert ( i2.search(10) == 0 );
	assert ( i2.max() == 1 );
	assert ( i2.min() == 8 );
	assert ( i2.has(8) );
	assert ( i2.has(5) );
	assert ( i2.has(1) );
	assert ( i2.has(6) );
	assert ( i2.has(3) );
	assert ( i2.has(-14) ==  false );
	assert ( i2.has(0) ==  false );
	assert ( i2.has(140) ==  false );

	#ifdef DEBUGGING
	std::cout << i2 <<std::endl;
	#endif

	Btree<int> i3;
	bool error_empty_tree = false;
	try{
		i3.max();
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_empty_tree = true;
	}
	assert ( error_empty_tree );
	assert ( i3.search(10) == 0 );
	assert ( i3.has(10) == false );
	assert ( i3.search(0) == 0 );
	assert ( i3.has(0) == false );


	Btree<int> i4 = i1.subtree(5);
	assert ( i4.search(1) != 0 );
	assert ( i4.search(0) == 0 );
	assert ( i4.max() == 6 );
	assert ( i4.min() == 1 );
	assert ( i4.has(1) );
	assert ( i4.has(5) );
	assert ( i4.has(6) );
	assert ( i4.has(3) );
	assert ( i4.has(16) == false );

}

void test_iterator(){
	std::cout << "TEST ITERATOR" << std::endl;

	Btree<int> r1 (4);
	r1.insert(3);
	r1.insert(5);
	r1.insert(8);
	r1.insert(-4);
	Btree<int>::const_iterator i;
	i = r1.begin();
	assert( i.get_value_it() == 4 );
	i++; //test operator++(int)
	assert( i.get_value_it() == 3 );
	i++; 
	assert( i.get_value_it() == 5 );
	++i; //test operator++()
	assert( i.get_value_it() == 8 );
	i++; 
	assert( i.get_value_it() == -4 );

	//test out_of_range_iterator
	bool error_iterator = false;
	try{
		i++;
	}
	catch(out_of_range_iterator e){
		//std::cerr << e.get_message() << std::endl;
		error_iterator = true;
	}
	assert ( error_iterator );

	Btree<int, decrescente<int> > r2; //, decrescente<int> 
	r2.insert(4);
	r2.insert(3);
	r2.insert(5);
	r2.insert(8);
	r2.insert(-4);
	Btree<int, decrescente<int> >::const_iterator j;


	int contatore = 0;
	for(j = r2.begin(); j != r2.end(); j++){
		switch(contatore){
			case 0 : assert( j.get_value_it() == 4 ); break;
			case 1 : assert( j.get_value_it() == 3 ); break;
			case 2 : assert( j.get_value_it() == 5 ); break;
			case 3 : assert( j.get_value_it() == 8 ); break;
			default : assert( false );
		}
		contatore++;
	} 
	assert( j.get_value_it() == -4 );

	Btree<int> bt(10);
	bt.insert(5);
	bt.insert(4);
	bt.insert(14);
	bt.insert(44);
	bt.insert(-3);
	bt.insert(6);
	bt.insert(2);
	assert( bt.size() == 8 );

	Btree<int>::const_iterator in, ie;

    in = bt.begin();
	ie = bt.end();

	int contatore_while = 0;
	while(in != ie){ 
		Btree<int>::Node node1(*in); 
		#ifdef DEBUGGING 
        std::cout  << *in << "|"; 
		#endif
		switch(contatore_while){ //test operator*()
			case 0 : assert( *in == 10 ); break;
			case 1 : assert( *in == 5 ); break;
			case 2 : assert( *in == 4 ); break;
			case 3 : assert( *in == 14 ); break;
			case 4 : assert( *in == 44 ); break;
			case 5 : assert( *in == -3 ); break;
			case 6 : assert( *in == 6 ); break;
			default : assert( false );
		}
		contatore_while++;
        in++ ; // testato anche i++
    }
	assert( *in == 2 );
	#ifdef DEBUGGING 
    std::cout  << *in << "|"; 
	std::cout << std::endl;
	#endif

	Btree<std::string>::const_iterator ite;
	Btree<std::string> StringTree("ciao");
	StringTree.insert("sono");
	StringTree.insert("un");
	StringTree.insert("albero");
	StringTree.insert("stringa");

	ite = StringTree.begin();
	assert( ite->size() == 4 ); //test operator->()
	ite++;
	assert( ite->size() == 4 );
	ite++;
	assert( ite->size() == 2 );

	#ifdef DEBUGGING 
    std::cout  << StringTree << std::endl;
	#endif
}

void test_stampa(){
	std::cout << "TEST STAMPA" << std::endl;

	Btree<int>::Node n1(7);
	std::cout << "Nodo : ";
	n1.stampa();
	std::cout << std::endl;

	Btree<int> r1 (4);
	r1.insert(3);
	r1.insert(5);
	r1.insert(8);
	r1.insert(-4);
	std::cout << "Size " << r1.size() << " ";
	std::cout << "Albero << : " << r1 << std::endl;
	std::cout << "Albero .stampa() : ";
	r1.stampa();

	pari<int> predicato_pari;
	std::cout << "Albero printIF pari : ";
	printIF(r1, predicato_pari);

	dispari<int> predicato_dispari;
	std::cout << "Albero printIF dispari : ";
	printIF(r1, predicato_dispari);

	Btree<int, decrescente<int> > r2 (4);
	r2.insert(3);
	r2.insert(5);
	r2.insert(8);
	r2.insert(-4);

	std::cout << "Albero decrescente .stampa() : ";
	r2.stampa();

	Btree<int> r3;
	std::cout << "Albero vuoto << : " << r3 << std::endl;
	std::cout << "Albero vuoto .stampa() : ";
	r3.stampa();

	Btree<int> r4 = r1.subtree(4);
	std::cout << "Size " << r4.size() << " ";
	std::cout << "Albero subtree << : " << r4 << std::endl;
	std::cout << "Albero subtree .stampa() : ";
	r4.stampa();

}

void test_subtree(){
	std::cout << "TEST SUBTREE" << std::endl;

	Btree<int> y (8);
	y.insert(5);
	y.insert(16);
	y.insert(6);
	y.insert(1);
	y.insert(3);
	#ifdef DEBUGGING
	std::cout<<"1.Y : ";
	y.stampa();
	#endif
	assert( y.size() == 6 );
	assert( y.get_root() == 8 );
	assert( y.get_last() == 3 );
	assert( y.max() == 16 );
	assert( y.min() == 1 );

	Btree<int> x = y.subtree(5);
	#ifdef DEBUGGING
	std::cout<< "1.X : ";
	x.stampa();
	#endif
	assert( y.size() == 6 );
	assert( y.get_root() == 8 );
	assert( y.get_last() == 3 );
	assert( y.max() == 16 );
	assert( y.min() == 1 );

	assert( x.size() == 4 );
	assert( x.get_root() == 5 );
	assert( x.get_last() == 6 );
	assert( x.max() == 6 );
	assert( x.min() == 1 );
	#ifdef DEBUGGING
	std::cout<<"1.1.X cout ";
	std::cout << x << std::endl; 
	#endif


	y.insert(4);
	assert( y.size() == 7 );
	assert( y.get_root() == 8 );
	assert( y.get_last() == 4 );
	assert( y.max() == 16 );
	assert( y.min() == 1 );

	assert( x.size() == 4 );
	assert( x.get_root() == 5 );
	assert( x.get_last() == 6 );
	assert( x.max() == 6 );
	assert( x.min() == 1 );
	#ifdef DEBUGGING
	std::cout<<"2.Y : ";
	y.stampa();

	std::cout<<"2.X : ";
	x.stampa();
	#endif
	x.insert(2);
	assert( y.size() == 7 );
	assert( y.get_root() == 8 );
	assert( y.get_last() == 4 );
	assert( y.max() == 16 );
	assert( y.min() == 1 );

	assert( x.size() == 5 );
	assert( x.get_root() == 5 );
	assert( x.get_last() == 2 );
	assert( x.max() == 6 );
	assert( x.min() == 1 );
	#ifdef DEBUGGING
	std::cout<<"3.Y : ";
	y.stampa();
	std::cout<<"3.X : ";
	x.stampa();
	#endif

	Btree<int> t1;
	Btree<int> t2 = t1.subtree(0);

	assert(t1.size() ==  0);

	bool error_root = false;
	try{
		t1.get_root(); //test get_root()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_root = true;
	}
	assert ( error_root );

	bool error_last = false;
	try{
		t1.get_last(); //test get_last()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_last = true;
	}
	assert ( error_last );


	Btree<int> t3 = y.subtree(15);

	assert(t3.size() ==  0);

	error_root = false;
	try{
		t3.get_root(); //test get_root()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_root = true;
	}
	assert ( error_root );

	error_last = false;
	try{
		t3.get_last(); //test get_last()
	}
	catch(uninitialized_value e){
		//std::cerr << e.get_message() << std::endl;
		error_last = true;
	}
	assert ( error_last );

	Btree<int> t4 = x.subtree(1);
	assert( t4.size() == 3 );
	assert( t4.get_root() == 1 );
	assert( t4.get_last() == 2 );
	assert( t4.max() == 3 );
	assert( t4.min() == 1 );

	assert( x.size() == 5 );
	assert( x.get_root() == 5 );
	assert( x.get_last() == 2 );
	assert( x.max() == 6 );
	assert( x.min() == 1 );
	#ifdef DEBUGGING
	std::cout<<"T4  : ";
	t4.stampa();
	#endif
}

void test_other_type(){
	std::cout << "TEST OTHER TYPE" << std::endl;

	Btree<char> c1('c');
	c1.insert('b');
	c1.insert('a');
	c1.insert('f');
	assert( c1.size() == 4 );
	assert( c1.has('c') );
	assert( c1.has('b') );
	assert( c1.has('a') );
	assert( c1.has('f') );
	assert( c1.has('z') == false );
	assert( c1.search('f') != 0 );
	assert( c1.search('z') == 0 );
	assert( c1.get_root() == 'c' );
	assert( c1.get_last() == 'f' );
	assert( c1.max() == 'f' );
	assert( c1.min() == 'a' );

	#ifdef DEBUGGING
	std::cout << "Albero char << : " << c1 << std::endl;
	std::cout << "Albero char .stampa() : ";
	c1.stampa();
	#endif

	Btree<char> c2 = c1.subtree('b');
	assert( c2.size() == 2 );
	assert( c2.has('b') );
	assert( c2.has('a') );
	assert( c2.has('f') == false );
	assert( c2.has('z') == false );
	assert( c2.search('f') == 0 );
	assert( c2.search('a') != 0 );
	assert( c2.get_root() == 'b' );
	assert( c2.get_last() == 'a' );
	assert( c2.max() == 'b' );
	assert( c2.min() == 'a' );


	Classe_custom custom1; //test Classe_custom()
	assert( custom1.get_numero() == 0 );
	assert( custom1.get_lettera() == 'a' );

	Classe_custom custom2(7, 'f');
	assert( custom2.get_numero() == 7 );
	assert( custom2.get_lettera() == 'f' );

	Classe_custom custom3(7, 'f');
	assert( custom3.get_numero() == 7 );
	assert( custom3.get_lettera() == 'f' );

	Classe_custom custom4(10, 'f');
	Classe_custom custom5(7, 'a');
	Classe_custom custom6(5, 'c');

	assert( custom2 == custom3 );
	assert( custom2 > custom1 );
	assert( custom1 < custom2 );
	assert( (custom2 < custom1) == false );

	Btree<Classe_custom> custom_tree(custom1);
	assert( custom_tree.size() == 1 );
	custom_tree.insert(custom2);
	custom_tree.insert(custom3);
	assert( custom_tree.size() == 2);
	custom_tree.insert(custom4);
	custom_tree.insert(custom5);
	custom_tree.insert(custom6);
	assert( custom_tree.size() == 5);
	assert( custom_tree.get_last().get_numero() == 5 );
	assert( custom_tree.get_root().get_numero() == 0 );
	assert( custom_tree.get_root().get_lettera() == 'a' );
	assert( custom_tree.max().get_numero() == 10 );
	assert( custom_tree.min().get_numero() == 0 );

	std::cout << "Albero custom << : "<< custom_tree << std::endl;
	std::cout << "Albero custom stampa : ";
	custom_tree.stampa();
	assert( custom_tree.has(custom3) );

	Btree<Classe_custom> custom_subtree = 
		custom_tree.subtree(custom2);
	
	assert( custom_subtree.size() == 4 );
	assert( custom_subtree.max().get_numero() == 10 );
	assert( custom_subtree.min().get_numero() == 5 );
	assert( custom_subtree.has(custom1) == false );
	assert( custom_subtree.has(custom4) );

}

void test_eccezioni(){
	std::cout << "TEST ECCEZIONI" << std::endl;

	
	Btree<int> r1 (4); //test out_of_range_iterator
	r1.insert(3);
	r1.insert(5);
	r1.insert(8);
	r1.insert(-4);
	Btree<int>::const_iterator i;
	i = r1.begin();
	assert( i.get_value_it() == 4 );
	i++; 
	assert( i.get_value_it() == 3 );
	i++; 
	assert( i.get_value_it() == 5 );
	++i; 
	assert( i.get_value_it() == 8 );
	i++; 
	assert( i.get_value_it() == -4 );

	//test out_of_range_iterator
	bool error_iterator = false;
	try{
		i++;
	}
	catch(out_of_range_iterator e){
		#ifdef DEBUGGING
		std::cerr << e.get_message() << std::endl;
		#endif
		error_iterator = true;
	}
	assert ( error_iterator );

	//test uninitialized_value
	Btree<int> t1; 
	assert(t1.size() ==  0);

	bool error_root = false;
	try{
		t1.get_root(); //test get_root() uninitialized_value
	}
	catch(uninitialized_value e){
		#ifdef DEBUGGING
		std::cerr << e.get_message() << std::endl;
		#endif
		error_root = true;
	}
	assert ( error_root );

	bool error_last = false;
	try{
		t1.get_last(); //test get_last() uninitialized_value
	}
	catch(uninitialized_value e){
		#ifdef DEBUGGING
		std::cerr << e.get_message() << std::endl;
		#endif
		error_last = true;
	}
	assert ( error_last );

	Btree<int>::Node *emptyNode = nullptr;
	
	bool error_node = false;
	try{
		emptyNode->get_value(); //test get_value() uninitialized_value
	}
	catch(uninitialized_value e){
		#ifdef DEBUGGING
		std::cerr << e.get_message() << std::endl;
		#endif
		error_node = true;
	}
	assert ( error_node );

}

int main(){


	test_Node();
	test_costruttor_Btree();
	test_my_functor();
	test_insert();
	test_search();
	test_iterator();
	test_stampa();
	test_subtree();
	test_other_type();
	test_eccezioni();

	std::cout <<"END TEST" << std::endl;

}	