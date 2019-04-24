#include "gamewidget.h"

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    this->universe = std::make_unique<Universe>(24,24);
    this->universe->setState(0,1,true);
    this->universe->setState(1,2,true);
    this->universe->setState(2,0,true);
    this->universe->setState(2,1,true);
    this->universe->setState(2,2,true);
    for (int i = 0; i < HIST_SIZE; ++i) {
        this->history[i] = std::unique_ptr<Universe>();
    }
    timer = std::make_unique<QTimer>(this);
    frameTime = std::make_unique<QTime>();
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(iterate()));
    this->iterationCount = 0;
}

GameWidget::~GameWidget()
{
    this->universe.release();
}

void GameWidget::iterate() {
    auto h = this->universe->getRows();
    auto w = this->universe->getCols();
    auto next = std::make_unique<Universe>(h, w);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            u_int alive_neighbours = 0;

            for (int row_neighbour = i-1; row_neighbour < i+2; ++row_neighbour) {
                for (int col_neighbour = j-1; col_neighbour < j+2; ++col_neighbour) {

                 if(row_neighbour == i && col_neighbour == j)
                    continue;
                 if(this->universe->getState( (row_neighbour+h) % h , (col_neighbour+w) % w ))
                       ++alive_neighbours;
                }
            }

            if(this->universe->getState(i, j))
            {
               if(alive_neighbours == 2 || alive_neighbours == 3)
                   next->setState(i,j,true);
                else
                   next->setState(i,j,false); // die of overpopulation or underpopulation
            }
            else {
                if(alive_neighbours == 3)
                    next->setState(i,j,true);
            }

        }
    }
    this->history[this->iterationCount % HIST_SIZE].reset(this->universe.release());
    this->iterationCount++;
    this->universe.reset(next.release());
    update();
    this->frameCount++;
    if (this->frameTime->elapsed() >= 1000)
     {
        double fps = this->frameCount / ((double) this->frameTime->elapsed() / 1000.0);
        this->frameCount = 0;
        this->frameTime->restart();
        emit updatefps(QString::number(fps, 'f', 0));
     }
}
void GameWidget::startGame() {
    this->timer->start();
    this->frameTime->start();
}

void GameWidget::stopGame() {
    this->timer->stop();
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawRect(0, 0, width()-1, height()-1);
    drawUniverse(p);

}

void GameWidget::drawUniverse(QPainter &p) {
        double cellWidth = (double) width() / this->universe->getCols();
        double cellHeight = (double) height() / this->universe->getRows();
        for (int i = 0; i < this->universe->getRows(); ++i) {
            for (int j = 0; j < this->universe->getCols(); ++j) {
                if(this->universe->getState(i,j)) {
                    double left = (double) (cellWidth*j);
                    double top = (double) (cellHeight*i);
                    QRectF r(left, top,  cellWidth, cellHeight);
                    p.fillRect(r, QBrush("#FF0000"));
                }
            }

        }
}

void GameWidget::loadRLE() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load .RLE", "*.rle");
    std::ifstream ifile(fileName.toUtf8().data());
    if(ifile.is_open())
    {
        std::string line;
        this->universe.reset(new Universe(100,100));
        int x = 0;
        int y = 0;
        while(ifile.good())
        {
            std::getline(ifile,line);
            if(line.front() == '#' || line.front() == 'x')
                continue;
            else
            {
                int amt = 0;
                for (auto c : line) {
                    if(c == 0xD || c == 0xA) continue;
                    if(std::isdigit(c))
                    {
                        amt = amt * 10 + int(c)-48;
                    } else {
                        if(amt == 0) amt = 1;
                        switch (c) {
                        case 'b':
                            this->universe->setState(y,x,false,amt);
                            break;
                         case 'o':
                            this->universe->setState(y,x,true,amt);
                            break;
                         case '$':
                            ++y;
                            x = 0;
                            amt = 0;
                            break;
                         case '!':
                            ifile.close();
                            this->update();
                            return;
                         default:
                            // terrible error catching
                            printf("%x\n",c);
                            exit(1);
                        }
                        x = x + amt;
                        amt = 0;
                    }
                }
            }
        }
        ifile.close();
        this->update();
    }
}

void GameWidget::saveRLE() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Pattern");
    std::ofstream ofile(fileName.toUtf8().data());
    int w = this->universe->getCols();
    int h = this->universe->getRows();

    if(ofile.is_open())
    {
        std::string line;
        std::string master;
        for (int row = 0; row < h; ++row) {
            line = "";
            bool last_symbol = false;
            int count = 0;
            int col = 0;
            while(col < w)
            {
                if(this->universe->getState(row,col) == last_symbol)
                {
                    ++count;
                    ++col;
                }
                else {
                    if(count > 0){
                        line += std::to_string(count);
                        line += last_symbol ? 'o' : 'b';
                    }
                    last_symbol = !last_symbol;
                    count = 0;
                }
            }
            line += '$';

            if(line.length() + master.length() > 70)
            {
                ofile << master << '\n';
                master = line;
            } else{
                master += line;
            }
        }
        ofile << master << '!' << '\n';
    }
    ofile.close();
}

void GameWidget::setFPS(QString text) {
    if(text.isEmpty())
    {
        this->timer->setInterval(1);
    } else if(text.toInt())
    {
        this->timer->setInterval( 1000 / text.toInt() );
    }

}

void GameWidget::rewind() {
    this->timer->stop();
    this->iterationCount--;
    this->universe.swap( this->history[ this->iterationCount % HIST_SIZE] );
    update();
}
