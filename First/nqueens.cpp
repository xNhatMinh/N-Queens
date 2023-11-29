#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace sf;

const int N = 8;
const int QUEEN_NUM = 1;
const int EMPTY = 0;
const int INVALID = 2;  // Thêm một giá trị mới cho ô không hợp lệ

struct Board {
    int coordinates[N][N];
    int size;
};

RenderWindow window(VideoMode(800, 800), "N-Queens Problem");

void render(const Board& board) {
    Texture queen;
    if (!queen.loadFromFile("Texture/queen.jpg")) {
        cout << "Error";
    }
    RectangleShape queenRect(Vector2f(100, 100));
    queenRect.setTexture(&queen);

    Texture check;
    if (!check.loadFromFile("Texture/check1.jpg")) {
        cout << "Error";
    }
    RectangleShape zarb(Vector2f(100, 100));
    zarb.setTexture(&check);

    window.clear();

    // Vẽ bảng cờ
    for (int i = 0; i < board.size; i++) {
        for (int j = 0; j < board.size; j++) {
            RectangleShape square(Vector2f(100, 100));
            square.setPosition(i * 100, j * 100);

            if (board.coordinates[i][j] == INVALID) {
                // đặt ô có ảnh là check
                zarb.setPosition(i * 100, j * 100);
                window.draw(zarb);
            }
            else if ((i + j) % 2 == 0) {
                square.setFillColor(Color::Yellow);
                window.draw(square);
            }
            else {
                square.setFillColor(Color::Blue);
                window.draw(square);
            }

            // Vẽ quân hậu
            if (board.coordinates[i][j] == QUEEN_NUM) {
                queenRect.setPosition(i * 100, j * 100);
                window.draw(queenRect);
            }
        }
    }

    window.display();
    sleep(seconds(1));  // Tạm dừng 1 giây để hiển thị từng bước
}

bool isValid(const Board& board, int row, int col) {
    // Kiểm tra dọc cột
    for (int i = 0; i < row; i++) {
        if (board.coordinates[i][col] == QUEEN_NUM) {
            return false;
        }
    }

    // Kiểm tra đường chéo trái trên
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board.coordinates[i][j] == QUEEN_NUM) {
            return false;
        }
    }

    // Kiểm tra đường chéo phải trên
    for (int i = row, j = col; i >= 0 && j < board.size; i--, j++) {
        if (board.coordinates[i][j] == QUEEN_NUM) {
            return false;
        }
    }

    return true;
}

void sfml(Board& board, int row, int queenColumn) {
    window.setFramerateLimit(1);  // Đặt giới hạn frame rate để thấy rõ từng bước

    if (row == board.size) {
        render(board);
        exit(EXIT_SUCCESS);
    }
    

    for (int col = 0; col < board.size; col++) {
        if (isValid(board, row, col)) {
            // Lưu trạng thái hiện tại của bảng
            Board tempBoard = board;
            

            // Di chuyển quân hậu đến vị trí mới
            board.coordinates[row][col] = QUEEN_NUM;
            
            render(board);

            sfml(board, row + 1, col + 1);
            // Quay lui: Đặt giá trị về trạng thái trước đó
            board = tempBoard;
        }
        else {
            board.coordinates[row][col] = INVALID;
            render(board);
            board.coordinates[row][col] = EMPTY;  // Xóa ô không hợp lệ sau khi hiển thị

        }
    }
}

void initialize(Board& board) {

    RenderWindow sizeWindow(VideoMode(800, 800), "Enter Chessboard Size", Style::Close);

    RectangleShape Background;
    Background.setSize(Vector2f(800, 800));
    Texture background_texture;
    background_texture.loadFromFile("Texture/Menu.png");
    Background.setTexture(&background_texture);

    Font font;
    if (!font.loadFromFile("curlz.ttf")) {
        cerr << "Error loading font\n";
        exit(EXIT_FAILURE);
    }

    Text welcome;
    welcome.setFont(font);
    welcome.setCharacterSize(35);
    welcome.setString("Welcome to my visualization of the N-Queens Problem!");
    welcome.setPosition(30, 100);

    Text prompt;
    prompt.setFont(font);
    prompt.setCharacterSize(35);
    prompt.setString("Enter the size of the chessboard (1 - " + to_string(N) + "):");
    prompt.setPosition(100, 200);

    Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(35);
    inputText.setPosition(625, 200);

    while (sizeWindow.isOpen()) {
        Event event;
        while (sizeWindow.pollEvent(event)) {
            if (event.type == Event::Closed) {
                sizeWindow.close();
                exit(EXIT_FAILURE);
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char typed = static_cast<char>(event.text.unicode);

                    if (typed >= '0' && typed <= '9') {
                        inputText.setString(inputText.getString() + typed);
                    }
                    else if (typed == '\b' && inputText.getString().getSize() > 0) {
                        inputText.setString(inputText.getString().substring(0, inputText.getString().getSize() - 1));
                    }
                }
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                stringstream ss(inputText.getString());
                ss >> board.size;

                if (board.size >= 1 && board.size <= N) {
                    sizeWindow.close();
                }
            }
        }

        sizeWindow.clear();
        sizeWindow.draw(Background);
        sizeWindow.draw(prompt);
        sizeWindow.draw(welcome);
        sizeWindow.draw(inputText);
        sizeWindow.display();
    }

    // Kiểm tra giá trị nhập vào có hợp lệ không
    if (board.size < 1 || board.size > N) {
        cout << "Invalid size. Exiting...\n";
        exit(EXIT_FAILURE);
    }

    memset(board.coordinates, 0, sizeof(board.coordinates));
}

int main() {
    Music music;
    if (!music.openFromFile("music/Queen.mp3")) {
        cout << "Error\n";
    }
    music.play();

    Board chessBoard;
    initialize(chessBoard);

    window.create(VideoMode(chessBoard.size * 100, chessBoard.size * 100), "N-Queens Problem");

    sfml(chessBoard, 0, 0);

    return 0;
}
