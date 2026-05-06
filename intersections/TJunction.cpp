#include "TJunction.h"
#include <random>
#include <iostream>

TJunction::TJunction() {
    std::vector<Vec2> travelPoints1;
    std::vector<Vec2> travelPoints2;
    std::vector<Vec2> travelPoints3;


    //z prawej do lewej
        travelPoints1 = {
            Vec2(100.0f, -1.75f),
            Vec2(100.0f, -1.75f),
            Vec2(90.0f, -1.75f),
            Vec2(60.0f, -1.75f),
            Vec2(30.0f, -1.75f),
            Vec2(5.0f, -1.75f),
             Vec2(-5.0f, -1.75f),

            Vec2(-30.0f, -1.75f),
            Vec2(-60.0f, -1.75f),
            Vec2(-90.0f, -1.75f),
            Vec2(-100.0f, -1.75f),
            Vec2(-100.0f, -1.75f)
        };
        travels.push_back(Travel(travelPoints1, 2, 1));
		//z lewej do prawej
        travelPoints2 = {
            Vec2(-100.0f, 1.75f),
            Vec2(-100.0f, 1.75f),
            Vec2(-90.0f, 1.75f),
            Vec2(-60.0f, 1.75f),

            Vec2(-40.0f, 1.75f),
            Vec2(-35.0f, 1.75f),
            Vec2(-30.0f, 3.5f),
            Vec2(-25.0f, 5.25f),
            Vec2(-20.0f, 5.25f),

            Vec2(-5.0f, 5.25f),
            Vec2(3.5f, 5.25f),
            Vec2(8.5f, 5.25f),
            Vec2(13.5f, 3.5f),
            Vec2(18.5f, 1.75f),
            Vec2(23.5f, 1.75f),

            Vec2(40.0f, 1.75f),
            Vec2(60.0f, 1.75f),
            Vec2(90.0f, 1.75f),
            Vec2(100.0f, 1.75f),
            Vec2(100.0f, 1.75f)
        };
        travels.push_back(Travel(travelPoints2, 2, 2));
		//z lewej w dol
        travelPoints3 = {
            Vec2(-100.0f, 1.75f),
            Vec2(-100.0f, 1.75f),
            Vec2(-90.0f, 1.75f),
            Vec2(-60.0f, 1.75f),
            Vec2(-30.0f, 1.75f),
            Vec2(-10.0f, 1.75f),

            Vec2(-2.5f, 1.75f),
            Vec2(0.0f, 1.75f),
            Vec2(1.75f, 1.75f),
            Vec2(1.75f, -3.5f),
            Vec2(1.75f, -5.0f),

            Vec2(1.75f, -5.0f),
            Vec2(1.75f, -20.0f),
            Vec2(1.75f, -50.0f),
            Vec2(1.75f, -70.0f),
            Vec2(1.75f, -100.0f),
            Vec2(1.75f, -100.0f),
        };
        travels.push_back(Travel(travelPoints3, 1, 3));

}
TJunction::TJunction(int index) {
    std::vector<Vec2> travelPoints1;
    std::vector<Vec2> travelPoints2;
    std::vector<Vec2> travelPoints3;
    std::vector<Vec2> travelPoints4;
    std::vector<Vec2> travelPoints5;
    std::vector<Vec2> travelPoints6;

    switch (index) {
        case 0:
            // Default T-junction
            break;
        case 1: //bezkonfliktowy prawoskret
            blocks.push_back(Block(-100.0f, -4.0f, 100.0f, -3.5f));
            blocks.push_back(Block(-100.0f, 3.5f, -3.5f, 4.0f));
            blocks.push_back(Block(3.5f, 3.5f, 100.0f, 4.0f));
            blocks.push_back(Block(-4.0f, 3.5f, -3.5f, 50.0f));
            blocks.push_back(Block(3.5f, 3.5f, 4.0f, 50.0f));
			//z lewej do prawej
            travelPoints1 = {
                Vec2(-90.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),
                Vec2(-30.0f, 1.75f),

                Vec2(-5.0f, 1.75f),
                Vec2(5.0f, 1.75f),

                Vec2(30.0f, 1.75f),
                Vec2(60.0f, 1.75f),
                Vec2(90.0f, 1.75f),
                Vec2(90.0f, 1.75f)
            };
            //z lewej w dol
            travels.push_back(Travel(travelPoints1, 2, 1));
            travelPoints2 = {
                Vec2(-90.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),
                Vec2(-30.0f, 1.75f),

                Vec2(-5.0f, 1.75f),
                Vec2(-1.75f, 1.75f),
                Vec2(-1.75f, 5.0f),

                Vec2(-1.75f, 15.0f),
                Vec2(-1.75f, 30.0f),
                Vec2(-1.75f, 45.0f),
                Vec2(-1.75f, 45.0f)
            };
            //z prawej do lewej
            travels.push_back(Travel(travelPoints2, 2, 2));
            travelPoints3 = {
                Vec2(90.0f, -1.75f),
                Vec2(90.0f, -1.75f),
                Vec2(60.0f, -1.75f),
                Vec2(30.0f, -1.75f),
                Vec2(15.0f, -1.75f),

                Vec2(5.0f, -1.75f),
                Vec2(-5.0f, -1.75f),

                Vec2(-30.0f, -1.75f),
                Vec2(-60.0f, -1.75f),
                Vec2(-90.0f, -1.75f),
                Vec2(-90.0f, -1.75f)
            };
            travels.push_back(Travel(travelPoints3, 1, 3));
			break;
        case 2: //lewoskret jeden konflikt
			//gorny z prawej do lewej
            blocks.push_back(Block(3.5f, 0.0f, 4.0f, 3.5f));
            blocks.push_back(Block(3.5f, 0.0f, 100.0f, 0.2f));
            blocks.push_back(Block(3.5f, 0.0f, 23.5f, 0.5f));
            blocks.push_back(Block(-100.0f, -4.0f, 0.0f, -3.5f));
            blocks.push_back(Block(-100.0f, 3.5f, -40.0f, 4.0f));
            blocks.push_back(Block(-100.0f, 0.0f, 0.0f, 0.2f));
            blocks.push_back(Block(-40.0f, 3.5f, -39.5f, 7.0f));
            blocks.push_back(Block(-40.0f, 7.0f, 23.5f, 7.5f));
            blocks.push_back(Block(23.5f, 3.5f, 24.0f, 7.5f));
            blocks.push_back(Block(23.5f, 3.5f, 100.0f, 4.0f));
            blocks.push_back(Block(3.5f, -4.0f, 100.0f, -3.5f));
            blocks.push_back(Block(3.5f, -100.0f, 4.0f, -3.5f));
            blocks.push_back(Block(-0.5f, -100.0f, 0.0f, -3.5f));
            blocks.push_back(Block(-20.0f, 3.5f, 3.5f, 3.7f));
            travelPoints1 = {
                Vec2(100.0f, -1.75f),
                Vec2(100.0f, -1.75f),
                Vec2(90.0f, -1.75f),
                Vec2(60.0f, -1.75f),
                Vec2(30.0f, -1.75f),
                Vec2(5.0f, -1.75f),
                 Vec2(-5.0f, -1.75f),

                Vec2(-30.0f, -1.75f),
                Vec2(-60.0f, -1.75f),
                Vec2(-90.0f, -1.75f),
                Vec2(-100.0f, -1.75f),
				Vec2(-100.0f, -1.75f)
            };
            //dolny z lewej do prawej
            travels.push_back(Travel(travelPoints1, 5, 1));
            travelPoints2 = {
                Vec2(-100.0f, 1.75f),
                Vec2(-100.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),

                Vec2(-40.0f, 1.75f),
                Vec2(-35.0f, 1.75f),
                Vec2(-30.0f, 3.5f),
                Vec2(-25.0f, 5.25f),
                Vec2(-20.0f, 5.25f),

                Vec2(-5.0f, 5.25f),
                Vec2(3.5f, 5.25f),
                Vec2(8.5f, 5.25f),
                Vec2(13.5f, 3.5f),
                Vec2(18.5f, 1.75f),
                Vec2(23.5f, 1.75f),
                
                Vec2(40.0f, 1.75f),
                Vec2(60.0f, 1.75f),
                Vec2(90.0f, 1.75f),
                Vec2(100.0f, 1.75f),
                Vec2(100.0f, 1.75f)
            };
			//srodkowy z lewej w gore
            travels.push_back(Travel(travelPoints2, 5, 2));
            travelPoints3 = {
                Vec2(-90.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),
                Vec2(-30.0f, 1.75f),
                //Vec2(-10.0f, 1.75f),

                Vec2(-1.5f, 1.75f),
                Vec2(1.75f, 1.75f),
                Vec2(1.75f, -1.5f),

                Vec2(1.75f, -30.0f),
                Vec2(1.75f, -60.0f),
                Vec2(1.75f, -90.0f),   
                Vec2(1.75f, -90.0f)
            };
            travels.push_back(Travel(travelPoints3, 1, 3));
			break;
		case 3: //pojedynczy pas, przejscie dla pieszych
            blocks.push_back(Block(-100.0f, -2.0f, 100.0f, -1.75f));
            blocks.push_back(Block(-100.0f, 1.75f, 100.0f, 2.0f));
            blocks.push_back(Block(0.0f, -1.75f, 5.0f, 1.75f, 10.0f, 5.0f));
            //z lewej do prawej
            travelPoints1 = {
                Vec2(-100.0f, 0.0f),
                Vec2(-100.0f, 0.0f),
                Vec2(-60.0f, 0.0f),
                Vec2(-30.0f, 0.0f),
                Vec2(0.0f, 0.0f),
                Vec2(30.0f, 0.0f),
                Vec2(60.0f, 0.0f),
                Vec2(100.0f, 0.0f),
                Vec2(100.0f, 0.0f)
            };
            travels.push_back(Travel(travelPoints1, 1, 1));
        case 4: //proste rondo

            //z gory w dol
            travelPoints1 = {
                Vec2(-100.0f, 0.0f),
            };
            //z gory w prawo
            travelPoints2 = {
                Vec2(-100.0f, 0.0f),
            };
            //z prawej w gore
            travelPoints3 = {
                Vec2(-100.0f, 0.0f),
            };
			//z prawej w dol
            travelPoints4 = {
                Vec2(-100.0f, 0.0f),
            };
			//z dolu w prawo
            travelPoints5 = {
                Vec2(-100.0f, 0.0f),
            };
			//z dolu w gore
            travelPoints6 = {
                Vec2(-100.0f, 0.0f),
            };
			travels.push_back(Travel(travelPoints1, 1, 1));
        default:
            std::cerr << "Unknown TJunction index: " << index << std::endl;
            break;
	}
}
std::vector<Block> TJunction::getBlocks() {
    return blocks;
}

Travel TJunction::getRandomTravel() {
    int sum_of_weight = 0;
    for (int i = 0; i < travels.size(); i++) {
        sum_of_weight += travels[i].getWeight();
    }
    int rnd = rand() % sum_of_weight;
    for (int i = 0; i < travels.size(); i++) {
        if (rnd < travels[i].getWeight())
            return travels[i];
        rnd -= travels[i].getWeight();
    }
}