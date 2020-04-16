#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

//#define DEBUGGING


/**
	@file Btree.h 
	@brief Dichiarazione della classe templata Btree
*/


/**
	Classe eccezione custom viene generata quando 
	si tenta di accedere a valori non
	ancora inizializzati

	@brief uninitialized value
*/
class uninitialized_value{
    
private:
    
    std::string message; 
public:
	uninitialized_value() {
		this->message = "Value not inzializated";
	}
    
	uninitialized_value(const std::string message) {
       this->message=message; 
    }
    std::string get_message() {
        return message;
    }
    
};

/**
	Classe eccezione custom che viene generata 
	quando si tenta di accedere ad indici maggiori 
	di end() con l'iteratore

	@brief  out of range iterator
*/
class out_of_range_iterator{
    
private:
    
    std::string message; 
public:
	out_of_range_iterator() {
		this->message = "Out of range with Iterator";
	}
    
	out_of_range_iterator(const std::string message) {
       this->message=message; 
    }
    std::string get_message() {
        return message;
    }
    
};


/**
	Funtore custom che serve per confrontare.
	func(a,b) == 0, se a = b
	func(a,b) == 1, se a < b
	func(a,b) == -1, se a > b

	@brief  crescente

	@param T tipo del dato template
*/
template <typename T>
struct crescente{
	int operator()(T a, T b) const {
		if( a == b ){
			return 0;
		}
		if( a < b ){
			return 1;
		}
		return -1;	
	}
};

/**
	Funtore custom che serve per confrontare.
	func(a,b) == 0, se a = b
	func(a,b) == -1, se a < b
	func(a,b) == 1, se a > b

	@brief  decrescente

	@param T tipo del dato template
*/
template <typename T>
struct decrescente{
	int operator()(T a, T b) const {
		if( a == b ){
			return 0;
		}
		if( a > b ){
			return 1;
		}
		return -1;	
	}
};

/**
	Funtore custom che serve per printIF.
	funct(A) = true, se A pari
	funct(A) = false, se A dispari

	@brief  pari

	@param T tipo del dato template
*/
template <typename T>
struct pari{
	bool operator()(T a) const {
		return (a % 2 == 0);	
	}
};

/**
	Funtore custom che serve per printIF.
	funct(A) = true, se A dispari
	funct(A) = false, se A pari

	@brief  dispari

	@param T tipo del dato template
*/
template <typename T>
struct dispari{
	bool operator()(T a) const {
		return (a % 2 != 0);	
	}
};

/**
	Classe che implementa un albero binario di dati generici T.  
    Gli elementi inseriti hanno valore di tipo T e sono inseriti 
	in modo ordinato. 
	L'ordinamento di default e' crescente ma puo' esser cambiato
	cambiando F.

	@brief Albero Binario

	@param T tipo del dato template

	@param F tipo del funtore per il confronto, default crescente.
	Dato  un fuct ti tipo F, allora funct(A,B) deve tornare 0 se
	A e' uguale a B, 1 se A e' minore di B e
	 -1 se A e' maggiore di B.
*/
template<typename T, typename F = crescente<T> >
class Btree{
public:

    /**
		Struttura che implementa un nodo dell'albero

		@brief nodo del'albero binario
	*/
	struct Node{
		T value; ///< dato inserito nel nodo
		F funct;
		Node *right; ///< puntatore al nodo destro
		Node *left; ///< puntatore al nodo sinistro
		Node *next;///< per l'iteratore


		/**
			@brief Costruttore default

			Costruttore di default che istanzia un nodo
			con value 0
		*/
		Node() : value(0), right(nullptr), left(nullptr), next(nullptr) {}

		/**
			@brief Costruttore 

			Costruttore che istanzia un nodo con 
			value v
			@param v valore del nodo
		*/
		Node(T v) : value(v), right(nullptr), left(nullptr), next(nullptr) {
			#ifdef DEBUGGING
			std::cout << "Node(T)" << std::endl;
			#endif
		}

		/**
			@brief Costruttore copia

			Costruttore che istanzia un nodo 
			copia di n
			@param n nodo da copiare
		*/
		Node(const Node &n) : value(n.value), next(nullptr) {
			#ifdef DEBUGGING
			std::cout << "Node(&)" << std::endl;
			#endif
			right = n.right->copy_node_ric();
			left = n.left->copy_node_ric();
			
			if( n.next != nullptr )
				force_next();
			
		}

		/**
			@brief Costruttore

			Costruttore che istanzia un nodo 
			copia shallow di n.
			ATTENZIONE il valore del primo nodo
			non e' shallow ma deep
			@param n nodo da copiare
		*/
		Node(Node *n) : value(n->value) { 
			//shallow copy (not the value of the root because is T)
			//NON vuole essere un copy-constructor
			#ifdef DEBUGGING
			std::cout << "Node(*)" << std::endl;
			#endif
			right = n->right;
			left =  n->left;
			next =  n->next;										
		}


	private:

		/**
			Funzione che crea una lista di nodi e la mette in array

			@brief Crea una lista di nodi e la mette in array

			@param array dove salva la lista
			@param i Nodo attuale da analizzare (start at 0)
			@param max lunghezza array
		*/
		void array_node_ric(Node *array[], unsigned int i, const unsigned int max){
			if(this != nullptr && i <= max ){

				//this->array_node(array, i+1, max);
				array[i] = this;

				if( this->left != nullptr ){
					this->left->array_node_ric(array, i+1, max);
				}
				else if( this->right != nullptr ){
					this->right->array_node_ric(array, i+1, max);
				}

				unsigned int sizeL = this->left->size();
				this->right->array_node_ric(array, i+sizeL+1, max);
				
			}			
		}

		/**
			Funzione che forza il ricalcolo di next

			@brief Forza il ricalcolo dei next di un insieme
			di nodi, serve per le subtree
			
			@param  n Nodo da cui parte il ricalcolo
			@param array Memorizza la lista di nodi
			@param dim Dimensione array
			@param i Indice corrente
		*/
		void force_next_ric(Node *n, Node *array[], const unsigned int dim, unsigned int i){
			if( i < dim ){
				n->next = array[i];
				force_next_ric(n->next, array, dim, i+1);
			}
			return;
		}

		/**
			Funzione che copia un nodo in modo ricorsivo
			ritornandone una copia deep.

			@brief Ritorna una copia deep del nodo
		*/
		Node* copy_node_ric(){
			if ( this == nullptr){
				return nullptr;
			}

			Node *nNode = new Node(this->value);
			const unsigned int sz_node = this->size();
			Node* array[ sz_node ];
			array_node(array, sz_node ); //Ho tutti i nodi un array
			for(int i = 0; i < sz_node; i++){
				nNode->insert_node(array[i]->get_value() );
			}
			//delete[] array;	
			return nNode;
		}

		/**
			Funzione che forza il ricalcolo di next

			@brief Forza il ricalcolo dei next di un insieme
			di nodi, serve per le subtree. 
			Usa force_next_ric e opera come
			this->force_next_ric(this, array, dim, 0);
		*/
		void force_next(){
			unsigned int dim = this->size();
			Node *array[dim];
			this->array_node(array, dim);
			this->force_next_ric(this, array, dim, 0);

			//delete [] array
		}

	public:
		
		/**
			Funzione inserisce un nodo.
			E' sempre meglio utilizzare la insert() presente
			nella classe Btree.

			@brief Inserisce un nodo in un insieme di nodi
			
			@param value Valore che sara' inserito
		*/
		void insert_node(T value) {
			#ifdef DEBUGGING
			std::cout << "Insert_node: " << value << std::endl;
			#endif
			if( funct( value, this->get_value() ) == 0 ) {
				return; 
			}
			if( funct( value, this->get_value() ) == 1 ) {
				if(this->left != nullptr)
					left->insert_node(value);
				else {
					try{
						this->left = new Node (value);
					}catch(...){
						this->left = nullptr; 
						throw;
					}
					//this->last->next = n->left;
					//this->last = n->left;
					/*			
					last = this;
					while( last->next != nullptr ){
						last = last->next;
					}
					*/

					this->next = this->left;	
				}  
			}
			else{
				if(this->right != nullptr)
					right->insert_node(value);
				else {
					try{
						this->right = new Node (value);	
					}catch(...){
						this->right = nullptr; 
						throw;
					}
					this->next = this->right;			
				}
			}
		}

		/**
			Funzione che copia un nodo
			ritornandone una copia deep.

			@brief Ritorna una copia deep del nodo
		*/
		Node* copy_node(){
			Node *n = this->copy_node_ric();
			//Node*n;
			//this->copy_node_ric(n);
			n->force_next();
			return n;
		}

		/**
			Funzione che distrugge l'insieme di nodi
			a partire da n.
			Attenzione funziona solo su puntatori!!

			@brief Distrugge l'insieme di nodi

			@param n Nodo da cui parte la distruzione
		*/
		void destroy(Node *n) {
			#ifdef DEBUGGING
			std::cout << "Size " << size() << std::endl;
			#endif //DEBUGGING
			if(n != nullptr) {
				#ifdef DEBUGGING
				std::cout << "Destroy NOT null" << std::endl;
				#endif //DEBUGGING
				if( n->left != nullptr )
					destroy(n->left);
				if( n->right != nullptr )
					destroy(n->right);
				delete n;
			}
			else{
				#ifdef DEBUGGING
				std::cout << "Destroy null" << std::endl;
				#endif //DEBUGGING
			}
		}

		/**
			Funzione che distrugge l'insieme di nodi.
			Attenzione funziona solo su puntatori!!

			@brief Distrugge l'insieme di nodi
		*/
		void destroy(){
			#ifdef DEBUGGING
			std::cout << "Size " << size() << std::endl;
			#endif //DEBUGGING
			if( this != nullptr) {
				#ifdef DEBUGGING
				std::cout << "Destroy NOT null" << std::endl;
				#endif //DEBUGGING
				if( this->left != nullptr )
					destroy(this->left);
				if( this->right != nullptr )
					destroy(this->right);
				delete this;
			}
			else{
				#ifdef DEBUGGING
				std::cout << "Destroy null" << std::endl;
				#endif //DEBUGGING
			}

		}

		/**
			Funzione che crea una lista di nodi e la mette in array

			@brief Crea una lista di nodi e la mette in array

			@param array dove salva la lista
			@param max lunghezza array
		*/
		void array_node(Node *array[], unsigned int max) {
			if( max >= 0 )
				array_node_ric(array, 0, max);
		}

		/**
			Funzione che calcola la dimensione
			dell'insieme di nodi

			@brief Ritorna la dimensione dell'insieme
			di nodi
		*/
		unsigned int size() const {
			if( this == nullptr ){
				return 0;
			}

			unsigned int sz = 1;
			return sz + this->left->size() + this->right->size();
		}

		/**
			Funzione getter per il valore di un nodo

			@brief Ritorna il value di un nodo

			@throw uninitialized_value
		*/
		T get_value() const { 
			if(this == nullptr){
				throw uninitialized_value();
			}
			return value;
		}

		/**
			Funzione che ritorna l'ultimo Next di un nodo

			@brief Ritorna l'ultimo next di un nodo

			@throw uninitialized_value
		*/
		Node last_next() {
			if(this == nullptr){
				throw uninitialized_value();
			}
			Node temp( this );
			while( temp.next != nullptr ){
				temp = temp.next;
			}
			//std::cout << temp.get_value() << std::endl;
			return temp;
		}

		/**
			Funzione che stampa l'insieme di nodi

			@brief Stampa dell'insieme dei nodi
		*/
		void stampa() const {
			if(this != nullptr){
				if( this->left != nullptr )
					this->left->stampa();

				std::cout << this->get_value() << "|";

				if( this->right != nullptr )
					this->right->stampa();
			}
			else{
				std::cout << "Empty Node";
			} 
		}

	};


private:
	//Attributi della classe
	unsigned int sz; ///< dimensione dell'albero
	Node *root; ///< radice dell'albero
	Node *last;	///< ultimo elemento dell'albero
	F funct; ///< funtore di confronto

	/**
		Funzione che distrugge l'insieme di nodi
		a partire da n

		@brief Distrugge l'insieme di nodi

		@param n Nodo da cui parte la distruzione
	*/
	void destroy(Node *n) {
			#ifdef DEBUGGING
			std::cout << "Size " << size() << std::endl;
			#endif //DEBUGGING
		if(n != nullptr) {
			#ifdef DEBUGGING
			std::cout << "Destroy NOT null" << std::endl;
			#endif //DEBUGGING

			destroy(n->left);
			destroy(n->right);
			decrease_size();
			delete n;
		}
		else{
			#ifdef DEBUGGING
			std::cout << "Destroy null" << std::endl;
			#endif //DEBUGGING
		}
	}

	/**
		Funzione che inserisce un nodo con
		valore value all'albero. Parte da n 
		a valutare l'albero.

		@brief Funzione di inserimento

		@param value Valore da inserire
		@param n Nodo da cui parte a inserire
	*/
	void insert(const T value, Node *n) {

		if( funct( value, n->get_value() ) == 0 ) {
			return; 
		}
		if( funct( value, n->get_value() ) == 1 ) {
			if(n->left != nullptr)
				insert(value, n->left);
			else {
				try{
					n->left = new Node (value);
				}catch(...){
					n->left = nullptr; 
					throw;
				}
				this->last->next = n->left;
				this->last = n->left;	
				sz++;
			}  
		}
		else{
			if(n->right != nullptr)
				insert(value, n->right);
			else {
				try{
					n->right = new Node (value);	
				}catch(...){
					n->right = nullptr; 
					throw;
				}
				this->last->next = n->right;
				this->last = n->right;			
				sz++;
			}
		}
	}

	/**
		Funzione che ricerca un valore 
		nell'albero a partire dal nodo n.
		Ritorna l'indirizzo del nodo, se presente, 
		altrimenti un puntatore nullptr

		@brief Cerca un valore nell'albero

		@param value Valore da cercare
		@param n Nodo da cui parte la ricerca
	*/
	Node *search(const T value, Node *n) const {
		if(n != nullptr) {
			if( funct( value, n->get_value() ) == 0 )
				return n;
			if( funct( value, n->get_value() ) == 1 )
				return search(value , n->left);
			else
				return search(value , n->right);
		}
		else{
			return nullptr;
		}
	}

	/**
		Funzione che decrementa la dimensione dell'albero

		@brief Decrementa la dimenzione dell'albero
	*/
	void decrease_size(){
		sz--;
	}

public:

	/**
		@brief Costruttore default

		Costruttore di default che istanzia un albero
		vuoto
	*/
	Btree() : sz(0), root(nullptr), last(nullptr) {}

	/**
		@brief Costruttore

		Costruttore che istanzia un albero 
		di dimensione 1 e con valore v
		@param v Valore della radice
	*/
	Btree(const T &v) : sz(1) {
		try{
			root = new Node(v);
		}catch(...){
			root = nullptr; 
			throw;
		}
		last = root;
	}

	/**
		@brief Costruttore

		Costruttore che istanzia un albero che
		ha come root n

		@param n Nodo da copiare e mettere come root
	*/
	Btree(Node n) {
		sz = n.size();
		root = n.copy_node();
		last = root;
		while( last->next != nullptr ){
			last = last->next;
		}
	}

	/**
		@brief Costruttore copia

		Costruttore che istanzia un albero
		come copia dell'albero other
		
		@param other albero da copiare
	*/
	Btree(const Btree<T,F> &other) {
		if( other.size() == 0 ){
			sz = 0;
			root = nullptr;
			last = nullptr;
		}
		else if( other.size() == 1 ){
			sz = 1;
			try{
				root = new Node( other.get_root() );
			}catch(...){
				root = nullptr; 
				throw;
			}
			last = root;
		}
		else{
			sz = other.size();
			root = other.root->copy_node();
			last = root;
			while( last->next != nullptr ){
				last = last->next;
			}
		}

	}

	/**
		@brief Distruttore

		Distruttore. 
		Rimuove la memoria allocata dall'albero.
	*/
	~Btree() {
		destroy();
		#ifdef DEBUGGING
		std::cout << "~Btree" << std::endl;
		#endif //DEBUGGING
	}
	
	/**
		@brief Operatore di assegnamento

        Operatore di assegnamento.
		Ridefinizione operator =.
		Permette la copia tra alberi.
		
		@param other albero da copiare
		@return reference a this
	*/
	Btree<T,F>& operator=(const Btree<T,F> &other){
		this->destroy();
		if( other.size() == 0 ){
			sz = 0;
			root = nullptr;
			last = nullptr;
		}
		else if( other.size() == 1 ){
			sz = 1;
			try{
				root = new Node( other.get_root() );
			}catch(...){
				root = nullptr; 
				throw;
			}
			last = root;
		}
		else{
			sz = other.size();
			root = other.root->copy_node();
			last = root;
			while( last->next != nullptr ){
				last = last->next;
			}
		}
		return *this;
	}
	
	/**
		Funzione che ritorna la dimensione dell'albero

		@brief Ritorna la dimensione dell'albero
	*/
	unsigned int size() const{
		return sz;
	}

	/**
		Funzione getter per il valore della root

		@brief Ritorna il value della root

		@throw uninitialized_value
	*/
	T get_root() const{
		if(root == nullptr){
			throw uninitialized_value();
		}
		return root->value;
	}

	/**
		Funzione getter per il valore dell'ultimo nodo next

		@brief Ritorna il value dell'ultimo nodo next

		@throw uninitialized_value
	*/
	T get_last() const{
		if(last == nullptr){
			throw uninitialized_value();
		}
		return last->value;
	}

	/**
		Funzione che ritorna il valore maggiore 
		di un insieme di nodi

		@brief Ritorna il value maggiore di un 
		insieme di nodi

		@throw uninitialized_value
	*/
	T max() const{
		if(root == nullptr){
			throw uninitialized_value();
		}
		Node temp( root );
		while( temp.right != nullptr ){
			temp = temp.right;
		}
		return temp.get_value();
	}

	/**
		Funzione che ritorna il valore minore 
		di un insieme di nodi

		@brief Ritorna il value minore di un 
		insieme di nodi

		@throw uninitialized_value
	*/
	T min() const{
		if(root == nullptr){
			throw uninitialized_value();
		}
		Node temp( root );
		while( temp.left != nullptr ){
			temp = temp.left;
		}
		return temp.get_value();
	}

	/**
		Funzione che inserisce un nodo con
		valore value all'albero.

		@brief Funzione di inserimento

		@param value Valore da inserire
	*/
	void insert(const T value) {
		if(root != nullptr)
			insert(value, root);
		else {
			try{
				root = new Node(value);
			}catch(...){
				root = nullptr; 
				throw;
			}
			last = root;
			sz++;
		}
		#ifdef DEBUGGING
		std::cout <<"Size: ";
		std::cout << sz << std::endl;
		#endif
	}

	/**
		Funzione che ricerca un valore 
		nell'albero.
		Ritorna l'indirizzo del nodo, se presente, 
		altrimenti un puntatore nullptr

		@brief Cerca un valore nell'albero

		@param value Valore da cercare
	*/
	Node *search(const T value) const{
		return search(value, root);
	}

	/**
		Funzione che rileva la presenza di 
		value nell'albero. Ritorna true se 
		value e' presente, altrimenti false.

		@brief Cerca un valore nell'albero

		@param value Valore da cercare
	*/
	bool has(const T value) const{
		return ( search(value) != 0 );
	}

	/**
		Funzione che distrugge l'insieme di nodi

		@brief Distrugge l'insieme di nodi
	*/
	void destroy() {
		destroy(root);
		//sz = 0;
		#ifdef DEBUGGING
		std::cout << "Size after destruction: " << size() << std::endl;
		#endif //DEBUGGING
	}

	/**
		Funzione crea e ritorna un sottoalbero con radice v. 
		Se v non e' presente ritorna un albero vuoto

		@brief Sottoalbero con radice v

		@param v Valore della radice del sottoalbero
	*/
	Btree<T,F> subtree(const T v){
		if( this->size() == 0 ){
			Btree<T, F> t;
			return t;
		}
		if( this->search(v) == nullptr ){
			Btree<T, F> t;
			return t;
		}

		//Btree<T, F> t ( this->search(v)->copy_node() );
		Btree<T, F> t ( this->search(v) );
		return t;
	}

	/**
		Funzione che stampa l' albero

		@brief Stampa dell'albero
	*/
	void stampa() const{
		if(root != nullptr){
			//std::cout<<"Btree: "<<std::endl;
			root->stampa();
			std::cout<< std::endl;
		} 
		else {
			std::cout<< "Empty Tree"<<std::endl;
		}
	}

	/**
		Iteratore costante dell' albero.

		@brief Iteratore costante dell' albero.
	*/
	class const_iterator {
		//	
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;


		/**
			Costruttore dell'iteratore costante
			@brief Mette il nodo a nulptr
		*/
		const_iterator() : ItPtr(nullptr) {}
		
		/**
			Costruttore copia dell'iteratore costante

			@brief Costruttore Copia 

			@param other Il const_iterator da copiare
		*/
		const_iterator(const const_iterator &other) : ItPtr(other.ItPtr) {}

		/**
			Ridefinizione operator =

			@brief Operatore di assegnamento
			@param other Un altro const_iterator other
			@return Riferimento all'iteratore
		*/
		const_iterator& operator=(const const_iterator &other) {
			ItPtr = other.ItPtr;
			return *this;
		}

		/**
			Distruttore dell'iteratore

			@brief Distruttore dell'iteratore
		*/
		~const_iterator() {}

		/** 
			Ritorna il dato riferito dal 
			const_iterator (dereferenziamento)
			
			@brief operatore di deferenziamento
		*/
		reference operator*() const {
			return ItPtr->value;
		}

		/** 
			Ritorna il dato riferito dal const_iterator 

			@brief operatore ->
		*/
		pointer operator->() const {
			return &(ItPtr->value);
		}
		
		/**
			Operatore di iterazione post-incremento

			@brief Operatore di iterazione post-incremento

			@throw out_of_range_iterator
		*/
		const_iterator operator++(int) {
			if( ItPtr->next == nullptr ){
				throw out_of_range_iterator();
			}

			const_iterator tmp(*this);
            ItPtr = ItPtr->next;
            return tmp;
		}

		/**
			Operatore di iterazione pre-incremento

			@brief Operatore di iterazione pre-incremento

			@throw out_of_range_iterator
		*/
		const_iterator& operator++() {
			if( ItPtr->next == nullptr ){
				throw out_of_range_iterator();
			}

			ItPtr = ItPtr -> next;
			return *this;
		}

		/** 
		 	Ridefinizione dell'operatore di uguaglianza

			@brief Operatore di uguaglianza

			@param other Un altro const_iterator
			@return Risultato dell'uguaglianza
		*/
		bool operator==(const const_iterator &other) const {
			return ItPtr == other.ItPtr;
		}
		
		/** 
		 	Ridefinizione dell'operatore di diseguaglianza

			@brief Operatore di uguaglianza

			@param other Un altro const_iterator
			@return Risultato della diseguaglianza
		*/
		bool operator!=(const const_iterator &other) const {
			return ItPtr != other.ItPtr;
		}

		/** 
		 	Funzione che ritorna il valore a cui 
			il const_iterator punta

			@brief Valore puntato dal const_iterator
		*/
		T get_value_it(){
			#ifdef DEBUGGING
			std::cout <<  this->ItPtr->value << std::endl;
			#endif
			return this->ItPtr->get_value();
		}


	private:
		const Node* ItPtr;
		friend class Btree; 

		const_iterator(const Node* n) : ItPtr(n) {}
		
		
	}; // classe const_iterator
	
	/**
		Ritorna l'iteratore all'inizio della sequenza dati
	
		@return Iteratore all'inizio della sequenza
	*/
	const_iterator begin() const {
		#ifdef DEBUGGING
		std::cout <<"Begin "<< root->value <<std::endl;
		#endif 
		return const_iterator(root);
	}
	
	/**
		Ritorna l'iteratore alla fine della sequenza dati
	
		@return Iteratore alla fine della sequenza
	*/
	const_iterator end() const {
		#ifdef DEBUGGING
		std::cout <<"End "<< last->value <<std::endl;
		#endif 
		return const_iterator(last);
	}
	
};


/**
    @brief Ridefinizione operatore <<


	Ridefinizione operatore <<.
	Permette dunque la stampa tramite std::ostream.

	@param out std::ostream scelto 
	@param t Un albero
*/
template<typename T, typename F>
std::ostream & operator << (std::ostream &out, Btree<T, F>& t) {
	if( t.size() != 0 ){
		typename Btree<T, F>::const_iterator j;
		unsigned int i = 0;
		for(j = t.begin(); i < t.size()-1 && j != t.end() ; j++, i++){
			out << j.get_value_it();
			out << "|";
		} 
		out << j.get_value_it();
		out << "|";
		return out;
	}
	out << "Empty Tree";
	return out;
}

/**
    @brief Stampa con predicato


	Stampa solo gli elementi dell'albero che soddisfano
	il predicato P

	@param t L'albero da stampare
	@param pred Il predicato binario (bool) 
*/
template<typename T, typename F, typename P>
void printIF(Btree<T, F>& t, P pred){
	if( t.size() != 0 ){
		typename Btree<T, F>::const_iterator j;
		for(j = t.begin(); j != t.end(); j++){
			if( pred( j.get_value_it() ) ){
				std::cout << j.get_value_it();
				std::cout << "|";
			}
		} 
		if( pred( j.get_value_it() ) ){
				std::cout << j.get_value_it();
				std::cout << "|";
		}
		std::cout<<std::endl;
	}
	else{
		std::cout << "Empty Tree" << std::endl;
	}
}

#endif //TREE_H