#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <time.h>
#include "AVl.h"
#include "Score.h"
using namespace std;
using namespace sf;
const int MAX_SIZE = 100;
const int INF = 1e9;
int vertices = 0;
int row, column;
char Maze[MAX_SIZE][MAX_SIZE];
int visitedPositions[MAX_SIZE][MAX_SIZE];
int count = 0;
int startPoint, endPoint;
bool InventoryInsertFlag = true;
int offsetX = 1;   // these tell that from where the maze will start draw like from cloumn 1 and row 13 maze[0][0] location occur
int offsetY = 13;
int No_of_Inventory = 150;
class Point {
public:
    int x, y;
    int enemies;
    int swords;
    int locks;
    int keys;

    Point() : x(0), y(0), enemies(0), swords(0), locks(0), keys(0) {}
    Point(int _x, int _y) : x(_x), y(_y), enemies(0), swords(0), locks(0), keys(0) {}

    // Define comparison operator for priority queue
    bool operator<(const Point& other) const {
        // Prioritize based on the number of enemies, swords, locks, and keys
        if (enemies != other.enemies) return enemies < other.enemies;
        if (swords != other.swords) return swords < other.swords;
        if (locks != other.locks) return locks < other.locks;
        if (keys != other.keys) return keys < other.keys;
        // If all counts are equal, prioritize based on distance
        return false;
    }
};


class pathNode {
public:
    int i;
    int j;
    pathNode* next;
    pathNode(int a, int b)
    {
        i = a; j = b;
        next = nullptr;
    }
};

class pathStore {
public:
    pathNode* head;
    int verticesN;

    pathStore()
    {
        head = nullptr;
        verticesN = 0;
    }

    void insert(int i, int j)
    {
        if (head == nullptr)
            head = new pathNode(i, j);
        else
        {
            pathNode* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = new pathNode(i, j);
        }
        verticesN++;
    }
};

sf::RenderWindow window(sf::VideoMode(1300, 860), "Maze Game");
sf::Clock clock1;
float timer = 0;
float Stimer = 0;

void GenerateMaze();
void display();
void drawMaze();
void ConnectTwoNodes(Point node1, Point node2);
Point GetRandomNeighbour(int i, int j);
bool isValid(int i, int j);
void Dijkstra(Point source, Point destination);
pathStore store1;

void createArrays() {
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            Maze[i][j] = '#';
            visitedPositions[i][j] = 0;
        }
    }
}

void Maze_to_make_differentPossiblePaths()
{
    srand(time(0));
    int x = 0;
    for (int i = 0; i < row - 3; i++)
        for (int j = 0; j < column - 2; j++)
        {
            if (i == 0 || j == 0)
                Maze[i][j] = '#';
            else
            {
                if (Maze[i][j] == '#' && Maze[i + 1][j] == '#' && Maze[i + 1][j + 1] == '#')
                {
                    x = rand() % 2 + 1;
                    if (x == 1)
                        Maze[i][j] = ' ';
                    else
                        Maze[i + 1][j + 1] = ' ';
                }
            }
        }
    
    int i = 0, j = 0,val;
    for(int k=0;k<No_of_Inventory;k++)
    {
        while(Maze[i][j] != ' ')
        {
            i = rand() % row + 1;
            j = rand() % column + 1;
        }
        val = rand() % 6+1;
        Maze[i][j] = static_cast<char>(val + '0');
    }
}

void Generation() {
    std::stack<Point> Nodes;
    Nodes.push({ 1, 1 });
    visitedPositions[1][1] = 1;

    while (!Nodes.empty()) {
        Point curNode = Nodes.top();
        Point nextNode = GetRandomNeighbour(curNode.x, curNode.y);
        if (nextNode.x == -1) {
            Nodes.pop();
        }
        else {
            visitedPositions[nextNode.x][nextNode.y] = 1;
            Nodes.push(nextNode);
            ConnectTwoNodes(curNode, nextNode);
        }
    }
    Maze_to_make_differentPossiblePaths();
}

void ConnectTwoNodes(Point node1, Point node2) {
    if (node1.x == node2.x) {
        for (int i = std::min(node2.y, node1.y); i <= std::max(node1.y, node2.y); i++) {
            Maze[node1.x][i] = ' ';
        }
    }
    else {
        for (int i = std::min(node2.x, node1.x); i <= std::max(node1.x, node2.x); i++) {
            Maze[i][node1.y] = ' ';
        }
    }
}

Point GetRandomNeighbour(int i, int j) {
    Point allNeighbours[4] = { Point(i, j + 2), Point(i, j - 2), Point(i + 2, j), Point(i - 2, j) };
    int count = 0;
    for (int k = 0; k < 4; k++) {
        int xc = allNeighbours[k].x, xy = allNeighbours[k].y;
        if (isValid(xc, xy) && !visitedPositions[xc][xy]) {
            allNeighbours[count++] = Point(xc, xy);
        }
    }
    if (count == 0) {
        return Point(-1, -1);
    }
    int random = std::rand() % count;
    return allNeighbours[random];
}

bool isValid(int i, int j) {
    return i >= 1 && j >= 1 && i < row - 1 && j < column - 1;
}

void GenerateMaze() 
{
    Generation();
    srand(time(0));

    //for random start and end points
    int temp = rand() % row;
    while(Maze[temp][1] == '#')
        temp = rand() % row;
     Maze[temp][0] = ' ';
     Maze[temp][0] = 'S';
     startPoint = temp;

     temp = rand() % row;
     while (Maze[temp][column-3] == '#')
         temp = rand() % row;
     Maze[temp][column - 3] = ' ';
     Maze[temp][column - 2] = 'e';
     endPoint = temp;
}

void display() {
    for (int i = 0; i < row ; i++) {
        for (int j = 0; j < column ; j++) {
            std::cout << Maze[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void drawMaze() {
    sf::RectangleShape wall(sf::Vector2f(20, 20));
    wall.setFillColor(sf::Color::Blue);
    for (int i = 0; i < row - 1; i++) {
        for (int j = 0; j < column - 1; j++) {
            if (Maze[i][j] == '#') {
                wall.setPosition((j * 20) + offsetX * 20, (i * 20) + offsetY * 20);
                window.draw(wall);
            }
        }
    }
}

void drawSD() {
    sf::CircleShape player(10);

    // Offset for starting position

    for (int i = 0; i < row - 1; i++) {
        for (int j = 0; j < column - 1; j++) {
            if (Maze[i][j] == 'S') {
                player.setFillColor(sf::Color::Green);
                player.setPosition((j * 20) + offsetX * 20, (i * 20) + offsetY * 20);
                window.draw(player);
            }
            else if (Maze[i][j] == 'e') {
                player.setFillColor(sf::Color::Red);
                player.setPosition((j * 20) + offsetX * 20, (i * 20) + offsetY * 20);
                window.draw(player);
            }
        }
    }
}

class Player {
public:
    sf::CircleShape S;
    char id;
    bool flag;

    Player()
    {
        flag = false;
    }

    void setAttr(int i,int j)
    {
        this->S.setFillColor(sf::Color::Yellow);
        this->S.setPosition((j * 20 + 5) + offsetX * 20, (i * 20 + 5) + offsetY * 20);
        this->S.setRadius(5);
        this->id = Maze[i][j];
        flag = true;
    }
};
bool playerFlag = true;
Player* P;
int currPlayer = 0;
pathNode* currPtr = nullptr;
float playerSpeed = 0.15;
void drawPath(float& tm) {
    // Offset for starting position
    if (playerFlag)
    {
        P = new Player[vertices]();
        playerFlag = false;
        currPtr = store1.head;
    }

    if (tm > playerSpeed && currPtr)
    {
        tm = 0;
        P[currPlayer].setAttr(currPtr->i, currPtr->j);
        currPtr = currPtr->next;
        currPlayer++;
    }

    for (int i = 0; i < currPlayer; i++)
    {
        if (P[i].flag)
            window.draw(P[i].S);
    }
}
void InsertInventory(Inventory*& root) {
    if (!InventoryInsertFlag)
        return;
    // Offset for starting position
    int offsetX = 1;
    int offsetY = 13;
    int count = 0;
    for (int i = 0; i < row - 1; i++) {
        for (int j = 0; j < column - 1; j++) {
            if (Maze[i][j] == '1') { 
                root = insert(root, count++, "img/sword.png", (j * 20 + 5) + offsetX * 20, (i * 20 + 5) + offsetY * 20,'1');
            }
            else if (Maze[i][j] == '2') {
                root = insert(root, count++, "img/lock.png", (j * 20 + 5) + offsetX * 20, (i * 20 + 5) + offsetY * 20,'2');
            }
            else if (Maze[i][j] == '3') {
                root = insert(root, count++, "img/key.png", (j * 20 + 5) + offsetX * 20, (i * 20 + 5) + offsetY * 20,'3');
            }
            else if (Maze[i][j] == '4') {
                root = insert(root, count++, "img/hat.png", (j * 20 + 5) + offsetX * 20, (i * 20 + 5) + offsetY * 20,'4');
            }
            else if (Maze[i][j] == '5') {
                root = insert(root, count++, "img/enemy.png", (j * 20 + 5) + offsetX * 20-3, (i * 20 + 5) + offsetY * 20-3,'5');
            }
            else if (Maze[i][j] == '6') {
                root = insert(root, count++, "img/drink.png", (j * 20 + 5) + offsetX * 20, (i * 20 + 5) + offsetY * 20,'6');
            }
        }
    }
    InventoryInsertFlag = false;
}

void Dijkstra(Point source, Point destination) {
    int distances[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            distances[i][j] = INF;
        }
    }

    std::priority_queue<std::tuple<int, Point>, std::vector<std::tuple<int, Point>>, std::greater<std::tuple<int, Point>>> pq;

    distances[source.x][source.y] = 0;
    pq.push({ 0, source });

    Point parent[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            parent[i][j] = Point(-1, -1);
        }
    }

    while (!pq.empty()) {
        Point curNode = std::get<1>(pq.top());
        int curDist = std::get<0>(pq.top());
        pq.pop();

        if (curNode.x == destination.x && curNode.y == destination.y) {
            // Path found, handle it according to instructions
            std::vector<Point> path;
            while (curNode.x != -1 && curNode.y != -1) {
                path.push_back(curNode);
                curNode = parent[curNode.x][curNode.y];
            }
            std::reverse(path.begin(), path.end());
            for (auto point : path) {
                vertices++;
                // Handle path nodes
                store1.insert(point.x, point.y);
            }
            return;
        }

        int dx[] = { 1, -1, 0, 0 };
        int dy[] = { 0, 0, 1, -1 };
        for (int k = 0; k < 4; k++) {
            int xc = dx[k] + curNode.x, xy = dy[k] + curNode.y;
            if (isValid(xc, xy) && Maze[xc][xy] != '#') {
                // Update distances and priority queue based on conditions
                int dist = curDist + 1;

                if (dist < distances[xc][xy]) {
                    distances[xc][xy] = dist;
                    pq.push({ dist, Point(xc, xy) });
                    parent[xc][xy] = curNode;
                }
            }
        }
    }
}



void TextualDisplay(Font &font1,Text* sc)
{
    font1.loadFromFile("img/font/1.otf");
    sc[0].setFont(font1);
    sc[0].setCharacterSize(70);
    sc[0].setFillColor(Color::Green);
    sc[0].setPosition(250, 3);
    sc[0].setString("Enchanted  Labyrinth  Explorer");

    sc[1].setFont(font1);
    sc[1].setCharacterSize(50);
    sc[1].setFillColor(Color::Green);
    sc[1].setPosition(900, 170);
    sc[1].setString("Score : ");

    sc[2].setFont(font1);
    sc[2].setCharacterSize(50);
    sc[2].setFillColor(Color::Green);
    sc[2].setPosition(60, 170);
    sc[2].setString("Inventory :  ");
}

void ScoreDisplay(Font &font,Text* sc,int val)
{
    font.loadFromFile("img/F3/f3.ttf");
    sc->setFont(font);
    sc->setCharacterSize(40);
    sc->setFillColor(Color::Red);
    sc->setPosition(1050, 180);
    sc->setString(to_string(val));

    window.draw(*sc);

}

int ItemposCount = 270;
void CheckCollision(Inventory* root, Player* player,Score& Sc) {
    if (root == nullptr || player == nullptr) return;
    if (root->left != nullptr) CheckCollision(root->left, player,Sc);

    float playerX = player->S.getPosition().x;
    float playerY = player->S.getPosition().y;
    int itemX = root->sprite.getPosition().x;
    int itemY = root->sprite.getPosition().y;
   // cout << itemX << " --- " << itemY << endl;

    float distance = sqrt(pow(playerX - itemX, 2) + pow(playerY - itemY, 2));
    float threshold = 10.0f; 
    if (distance < threshold) {
        cout << root->val <<" ->  Collion Detected\n";
        root->sprite.setPosition(ItemposCount, 190);
        root->sprite.setScale(root->sprite.getScale().x+0.5,root->sprite.getScale().y+0.5);
        ItemposCount += 40;
        Sc.scoreUpdate(root->identifier);
        return;
    }

    if (root->right != nullptr && player) CheckCollision(root->right, player,Sc);
}


int main() {

    Score Sc;
    std::srand(std::time(nullptr));
    Inventory* root = nullptr;
    
    row = 30;
    column = 64;

    createArrays();
    GenerateMaze();

    Point source = { startPoint, 1 };
    Point destination = { endPoint, column - 3 };
    Dijkstra(source, destination);
    Font font[2];
    Text tx[3];
    Text score;
    TextualDisplay(font[0], tx);
    InsertInventory(root);
    store1.head;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float time = clock1.getElapsedTime().asSeconds();
        clock1.restart();
        timer += time;
        window.clear(sf::Color::Black);
        for(int i=0;i<3;i++)
        window.draw(tx[i]);
        drawSD();
        drawMaze();
        PreOrder(root, window);
        drawPath(timer);
        ScoreDisplay(font[1],&score,Sc.getScore());
        if (Sc.getScore() < 0)
        {
            cout << "Score is "<<Sc.getScore()<<" which is less Than 0\n";
            return 0;
        }
        for (int i = 0; i < vertices; i++)
            CheckCollision(root, &P[i],Sc);
        window.display();
    }
    cout << "Minimum Vertices : " << ::vertices << endl;
   
    return 0;
}
