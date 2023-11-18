#include "root.h"
#include "iostream"


int main() {
	srand(time(NULL));
	ARoot root;
	if (!root.Init()) {
		return -1;
	}
	root.Run();
	return 0;
}

//int main() {
//	int a = 33;
//	int b = floor(a / 16);
//	std::cout << b << std::endl;
//	Sleep(20);
//}

//int main() {
//	setlocale(LC_ALL, "en_us.utf8");
//	srand(time(NULL));
//	ADeck* deck = new ADeck();
//	deck->Init();
//	deck->Shuffle();
//	for (short i = 0; i < 36; i++) {
//		std::cout << deck->Ranks[deck->Deck[i].Rank] << " of " << deck->Suits[deck->Deck[i].Suit] << "; TRUMP: " << deck->Deck[i].Trump << std::endl;
//	}
//	delete deck;
//	deck = nullptr;
//	return 0;
//}