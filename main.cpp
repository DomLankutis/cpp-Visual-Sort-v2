#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "./globals.h"

void setRectangleArray(){
    for (int i = 0; i <= sizeOfArray; i++) {
        float gap = (sizeOfArray + 1) / WIDTH;
        float rectangleWidth = (WIDTH - gap) / sizeOfArray;
        float rectangleHeigh = HEIGHT / (sizeOfArray + 1);
        sf::RectangleShape rectangle(sf::Vector2f(rectangleWidth, -(arr[i] * rectangleHeigh)));
        rectangle.setPosition((rectangleWidth + gap) * i, HEIGHT);
        rectangles[i] = rectangle;
    }
}

void draw(sf::RenderWindow &window) {
    window.clear();
    for(int i = 0; i < sizeOfArray; i++) {
        window.draw(rectangles[i]);
    }
    
    window.display();
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
    setRectangleArray();
}

void color(int index, sf::Color color) {
    rectangles[index].setFillColor(color);
}

int playSound(double freq) {
    const unsigned SAMPLES = 44100;
	const unsigned SAMPLE_RATE = 44100;
	const unsigned AMPLITUDE = 20000;
	
	sf::Int16 raw[SAMPLES];

	const double TWO_PI = 6.28318;
	const double increment = (44100. / freq) / 44100;
	double x = 0;
	for (unsigned i = 0; i < SAMPLES; i++) {
		raw[i] = AMPLITUDE * sin(x*TWO_PI);
		x += increment;
	}
	
	sf::SoundBuffer Buffer;
	if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		return 1;
	}

	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.play();
    sf::sleep(sf::milliseconds(5));
}

void bubbleSort(int arr[], sf::RenderWindow &window) {
    for (int i = 0; i < sizeOfArray; i++) {
        int swaps = 0;
        for (int j = 0; j < sizeOfArray - i; j++) {
            if (arr[j] > arr[j + 1]){
                swap(&arr[j], &arr[j + 1]);
                swaps += 1;
                color(j + 1, sf::Color::Red);
                // playSound(j);
            }
            draw(window);
        }
        if (swaps == 0){
            break;
        }
    }
}

void insertionSort(int arr[], sf::RenderWindow &window) {
    for(auto i = 1; i < sizeOfArray; i++) {
        auto j = i;
        while (j > 0 && arr[j] < arr[j - 1]) {
            swap(&arr[j], &arr[j - 1]);
            color(j, sf::Color::Red);
            // playSound(j);
            draw(window);
            j--;
        }
    }
}

int partition(int arr[], int low, int high, sf::RenderWindow &window) {
    // Pivot is determined by median of three
    int pivot = arr[(low + (low + (high - low) / 2) + high) / 3];
    int i = low, j = high + 1;

    while (true) {
        while (arr[i] < pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i >= j){
            setRectangleArray();
            draw(window);
            return j;
        }
        swap(&arr[i], &arr[j]);
        color(pivot, sf::Color::Green);
        color(arr[i], sf::Color::Red);
        color(arr[j], sf::Color::Blue); 
 //       playSound(i);
 //       playSound(j);
        draw(window);
    }
}

void quickSort(int arr[], int low, int high, sf::RenderWindow &window) {
    if (low < high) {
        int partitionIndex = partition(arr, low, high, window);
       
        quickSort(arr, low, partitionIndex, window);
        quickSort(arr, partitionIndex + 1, high, window);
    }
}

int main() {
    // Fill the array with values
    for(int i = 0; i <= sizeOfArray; i++){
        arr[i] = i+1;
    }
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "new attempt", sf::Style::Titlebar);

    while (window.isOpen()){
        sf::Event event;
        
        while (window.pollEvent(event)){
            if (event.type == sf::Event::EventType::Closed) {
                window.close();
            }

            if (event.type == sf::Event::EventType::KeyPressed) {
                srand(time(0));
                std::random_shuffle(std::begin(arr), std::end(arr));

                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                
                if (event.key.code == sf::Keyboard::B) {
                    bubbleSort(arr, window);
                }

                if (event.key.code == sf::Keyboard::I) {
                    insertionSort(arr, window);
                }

                if (event.key.code == sf::Keyboard::Q) {
                    quickSort(arr,0, sizeOfArray, window);                    
                }
            }
        }
     }
}
