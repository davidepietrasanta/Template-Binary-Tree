# MODE = -DNDEBUG #per compilare in modalita' release

 MODE =  # per compilare in modalita' debug

tree: main.cpp
	g++ $(MODE) -std=c++0x main.cpp -o tree

.PHONY: clean

clean:
	rm *.exe *.o
