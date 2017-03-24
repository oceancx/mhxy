/******************************************************************/
#include "Game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "MAP.h"
#include "mhxy.h"
#include "WDF.h"
#include "Sprite2.h"
#include <vector>
#include <cmath>
#include "Config.h"
#include <iostream>
#include <SOIL/SOIL.h>
#include "GameMap.h"
#include <algorithm>
#include <list>

// Game-related State data
SpriteRenderer  *Renderer;
std::vector<Texture2D> mMapTiles;
std::vector<Texture2D> mMaskTiles;
std::vector<vector<Texture2D>> mSprites;

Sprite2 sprite, sprite2;


int cur_frame = 0;
int frame_count;
int dir_count = 0;
GLfloat delta = 0;

int dir = 0;
//右下，左下，左上，右上，下，左，上，右
int dirs[8] = { 0,1,2,3,4,5,6,7 };

//↑ → ↓ ← ↗ ↘ ↙ ↖
int dir_x[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
int dir_y[8] = { -1, 0, 1, 0,-1, 1, 1,-1 };



int m_RowCount = 0;
int m_ColCount = 0;
int src_x , src_y ;
int cur_x, cur_y;
int dest_x = src_x, dest_y= src_y;
NetEase::MAP *gameMap;
int** cells;

Texture2D* cellPic;
GameMap* mGameMap;

std::list<GameMap::Pos> posList;

Game::Game(GLuint width, GLuint height)
: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // printf("init\n");
    // Load shaders
    //ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("sprite.vs", "sprite.frag", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Set render-specific controls
    Shader shader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(shader);
    /*读取配置*/
    Config config("config.txt");
    std::cout << config.mRows[0].path << std::endl;
    gameMap = new NetEase::MAP(config.mRows[0].path);
    m_RowCount = gameMap->m_RowCount;
    m_ColCount = gameMap->m_ColCount;
    std::cout << "row:" << m_RowCount << " col:" << m_ColCount << std::endl;


    /*为地图障碍分配内存*/
    cells = new int*[m_RowCount*m_ColCount];
    for (int i = 0; i<m_RowCount*m_ColCount; i++) {
        cells[i] = new int[192];
    }

    /*读取地图 以及 地图障碍*/
    for (int i = 0; i<m_RowCount; i++) {
        for (int j = 0; j<m_ColCount; j++) {
            gameMap->ReadUnit(i*m_ColCount + j);
            for (int k = 0; k<192; k++) {
                cells[i*m_ColCount + j][k] = (gameMap->m_MapUnits)[i*m_ColCount + j].Cell[k];
            }

            Texture2D texture;
            texture.Generate(320, 240, (gameMap->m_MapUnits)[i*m_ColCount + j].BitmapRGB24);
            mMapTiles.push_back(texture);
        }
    }

    for (int i = 0; i < gameMap->m_MaskSize; i++)
    {
        gameMap->ReadMask(i);
        Texture2D texture;
        texture.Generate2(gameMap->m_MaskInfos[i].Width,
                          gameMap->m_MaskInfos[i].Height,
                          (unsigned char*)gameMap->m_MaskInfos[i].Data);
        mMaskTiles.push_back(texture);
    }

    mGameMap = new GameMap(gameMap->m_Width,gameMap->m_Height, m_RowCount, m_ColCount, cells);
    posList=std::list<GameMap::Pos>();
    mGameMap->Move(24, 15, 30, 20,posList);
    src_x = posList.front().x * 20 +10;
    src_y = posList.front().y * 20 +10;
    cur_x = src_x;
    cur_y = src_y;
    posList.pop_front();
    dest_x= posList.front().x*20 +10;
    dest_y = posList.front().y * 20+10;


    /*初始化障碍图片*/
    int width, height;
    unsigned char* image = SOIL_load_image("X.png", &width, &height, 0,
                                           SOIL_LOAD_RGBA);
    printf("%d %d\n", width, height);
    cellPic = new Texture2D();
    cellPic->Generate2(20, 20, image);
    SOIL_free_image_data(image);



    /*读取人物Sprite*/
    NetEase::WDF wdf(config.mRows[1].path);
    sprite = wdf.LoadSprite(config.mRows[1].params[0]);
    std::vector<Texture2D> tSprit;
    dir_count = sprite.mGroupSize;
    for (int i = 0; i<sprite.mFrameSize*sprite.mGroupSize; i++) {
        int gpos = i / sprite.mFrameSize;
        int cpos = i%sprite.mFrameSize;

        Texture2D texture;
        texture.Generate2(sprite.mWidth, sprite.mHeight, (unsigned char*)&sprite.mFrames[gpos][cpos].src[0]);
        tSprit.push_back(texture);
    }
    mSprites.push_back(tSprit);
    /*读取武器Spirte*/
    tSprit.clear();
    NetEase::WDF wdf2(config.mRows[2].path);
    sprite2 = wdf2.LoadSprite(config.mRows[2].params[0]);
    for (int i = 0; i<sprite2.mFrameSize*sprite2.mGroupSize; i++) {
        int gpos = i / sprite2.mFrameSize;
        int cpos = i%sprite2.mFrameSize;

        Texture2D texture;
        texture.Generate2(sprite2.mWidth, sprite2.mHeight, (unsigned char*)&sprite2.mFrames[gpos][cpos].src[0]);
        tSprit.push_back(texture);
    }
    mSprites.push_back(tSprit);

    frame_count = sprite.mFrameSize;


}

GLfloat delta2=0;
void Game::Update(GLfloat dt)
{
    delta += dt;
    delta2 += dt;

    if (delta2 >= 8 * dt) {
        delta2 = 0;
        cur_frame++;
        if (cur_frame >= frame_count) {
            cur_frame = 0;
        }
    }

    if (delta >=  dt) {
        //printf("%lf", 8 * dt);
        delta = 0;
        double dx = max(1.0,1.0* (dest_x - src_x));
        double dy = dest_y - src_y;
        double angle;
        double degree;
        angle = atan(dy / dx);
        degree = angle * 180 / 3.1415;
        //  printf("angel: %lf \n",angle *180/3.1415);
        if (dx < 0 && dy > 0) {
            degree += 180;
        }
        else if (dx<0 && dy<0) {
            degree += 180;
        }
        else if (dx>0 && dy<0) {
            degree += 360;
        }
        else if (dx>0 && dy>0) {

        }
        //右下，左下，左上，右上，下，左，上，右
        // 0    1    2    3   4  5   6  7
        if (degree >= 360 - 22.5 || degree <= 22.5) {
            dir = 7;
        }
        else if (degree>22.5 && degree <= 22.5 + 45) {
            dir = 0;
        }
        else if (degree>22.5 + 45 && degree <= 90 + 22.5) {
            dir = 4;
        }
        else if (degree>22.5 + 90 && degree <= 135 + 22.5) {
            dir = 1;
        }
        else if (degree>22.5 + 135 && degree <= 180 + 22.5) {
            dir = 5;
        }
        else if (degree>22.5 + 180 && degree <= 225 + 22.5) {
            dir = 2;
        }
        else if (degree>22.5 + 225 && degree <= 270 + 22.5) {
            dir = 6;
        }
        else if (degree>22.5 + 270 && degree <= 315 + 22.5) {
            dir = 3;
        }

        if (!posList.empty())
        {
            if ( cur_x  == dest_x &&  cur_y == dest_y )
            {
                src_x = dest_x;
                src_y = dest_y;
                posList.pop_front();
                if (!posList.empty()) {
                    dest_x = posList.front().x * 20 +10;
                    dest_y = posList.front().y * 20 +10;
                }
                else {
                    dest_x = cur_x;
                    dest_y = cur_y;
                }
            }
            else
            {
                //右下，左下，左上，右上，下，左，上，右
                // 0    1    2    3   4  5   6  7
                int dis_dx =3*dt*100;
                int dis_dy =3*dt*100;
                if (dir == 0) {
                    cur_x += dis_dx;
                    cur_y += dis_dy;
                }
                else if (dir == 1) {
                    cur_x -= dis_dx;
                    cur_y += dis_dy;
                }
                else if (dir == 2) {
                    cur_x -= dis_dx;
                    cur_y -= dis_dy;
                }
                else if (dir == 3) {
                    cur_x += dis_dx;
                    cur_y -= dis_dy;
                }
                else if (dir == 4) {
                    cur_y += dis_dy;
                    //cur_x = dest_x;
                }
                else if (dir == 5) {
                    cur_x -= dis_dx;

                    //	cur_y = dest_y;
                }
                else if (dir == 6) {
                    cur_y -= dis_dy;

                    //cur_x = dest_x;
                }
                else if (dir == 7) {
                    cur_x += dis_dx;

                    //	cur_y = dest_y;
                }

                if ( abs(cur_x - dest_x) < dis_dx ){
                    cur_x = dest_x;
                }
                if ( abs(cur_y - dest_y)  < dis_dy ){
                    cur_y= dest_y;
                }
                if (dir == 3 && (cur_x - dest_x) == 0) {
                    printf(" %d %d\n", cur_x, cur_y);
                    printf("src x:%d src y:%d\n", src_x, src_y);
                    printf("dest x:%d dest y:%d\n", dest_x, dest_y);
                }

            }
        }
        else {
            mGameMap->Move(24, 15, 255, 34, posList);
            src_x = posList.front().x * 20 + 10;
            src_y = posList.front().y * 20 + 10;
            cur_x = src_x;
            cur_y = src_y;
            posList.pop_front();
            dest_x = posList.front().x * 20 + 10;
            dest_y = posList.front().y * 20 + 10;
        }





        /*
         int gpos = cur_frame / frame_count;
         int cpos = cur_frame%frame_count;
         Sprite2::Sequence& frame1 = sprite.mFrames[gpos][cpos];
         Sprite2::Sequence& frame2 = sprite2.mFrames[gpos][cpos];
         printf("gpos: %d cpos: %d \n",gpos,cpos);
         printf("w:%d h:%d key1=> x:%d y:%d sprite1[w:%d h:%d key=> x:%d y:%d]\n",
         frame1.width,frame1.height,  frame1.key_x,frame1.key_y,sprite.mWidth,sprite.mHeight, sprite.mKeyX,sprite.mKeyY );
         printf("w:%d h:%d key2=> x:%d y:%d sprite2[w:%d h:%d key=> x:%d y:%d]\n",
         frame2.width,frame2.height,  frame2.key_x,frame2.key_y,sprite2.mWidth,sprite2.mHeight,  sprite2.mKeyX,sprite2.mKeyY);
         */
    }



}

void Game::mouse_button_click()
{
    //printf("%lf %lf clicked!\n",mouse_x,mouse_y );
    // player_dest_x=mouse_x;
    // player_dest_y=mouse_y;

}
void Game::ProcessInput(GLfloat dt)
{


    // Move playerboard
    if (this->Keys[GLFW_KEY_A])
    {
        cur_x -= 1;
        // if(src_x<=-m_MapWidth+1024)src_x=-m_MapWidth+1024;
    }
    if (this->Keys[GLFW_KEY_D])
    {
        cur_x += 1;
        // if(src_x>=0)src_x=0;

    }

    if (this->Keys[GLFW_KEY_W])
    {
        cur_y -= 1;
        // if(src_y<=-m_MapHeight+768)src_y=-m_MapHeight+768;
    }

    if (this->Keys[GLFW_KEY_S])
    {
        cur_y += 1;
        // if(src_y>=0)src_y=0;

    }


}

void Game::Render()
{

    int cut_row = cur_y / 240;
    int cut_col = cur_x / 320;

    for (int i = 0; i<m_RowCount; i++) {
        for (int j = 0; j<m_ColCount; j++) {
            if ((i > cut_row - 2 || i < cut_row + 2) &&
                (j>cut_col - 2 || j<cut_col + 2)) {
                Renderer->DrawSprite(mMapTiles[i*m_ColCount + j],
                                     glm::vec2(j * 320 + 320+ sprite.mWidth / 2 - cur_x ,
                                               i * 240 + 240 + sprite.mHeight / 2  - cur_y), glm::vec2(320, 240), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
            
        }
    }
    
    int px = 320 - sprite.mWidth/2 ;
    int py = 240 - sprite.mHeight/2;
    
    int gpos = cur_frame / frame_count;
    int cpos = cur_frame%frame_count;
    Sprite2::Sequence& frame1 = sprite.mFrames[gpos][cpos];
    Sprite2::Sequence& frame2 = sprite2.mFrames[gpos][cpos];
    
    
    Renderer->DrawSprite(mSprites[0][cur_frame + dir*frame_count],
                         glm::vec2(px, py),
                         glm::vec2(sprite.mWidth, sprite.mHeight), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    
    // int px2 =   px-(frame2.key_x-frame1.key_x -( sprite2.mKeyX - sprite.mKeyX));  //-(frame2.key_x-frame1.key_x);
    int px2 = px - (sprite2.mKeyX - sprite.mKeyX);  //-(frame2.key_x-frame1.key_x);
    // int py2 =  py-(frame2.key_y- frame1.key_y - ( sprite2.mKeyY -  sprite.mKeyY));//- (frame2.key_y-frame1.key_y);
    int py2 = py - (sprite2.mKeyY - sprite.mKeyY);//- (frame2.key_y-frame1.key_y);
    // printf("px2: %d  py2: %d\n", px2,py2);
    Renderer->DrawSprite(mSprites[1][cur_frame + dir*frame_count],
                         // - (sprite2.mKeyX-frame2.key_x),
                         //- (sprite2.mKeyY-frame2.key_y)), 
                         glm::vec2(
                                   px2, py2
                                   ),
                         glm::vec2(sprite2.mWidth, sprite2.mHeight), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    
    for (int m= 0; m < gameMap->m_MaskSize; m++) 
    {
        NetEase::MaskInfo& info = gameMap->m_MaskInfos[m];
        Renderer->DrawSprite(
                             mMaskTiles[m],
                             glm::vec2(info.StartX + 320+ sprite.mWidth/ 2 - cur_x,
                                       info.StartY + 240 +sprite.mHeight / 2 - cur_y),
                             glm::vec2(info.Width, info.Height),
                             0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
    
    /*
     for (int i = 0; i<m_RowCount; i++) {
     for (int j = 0; j<m_ColCount; j++) {
     if ((i > cut_row - 2 || i < cut_row + 2) &&
     (j > cut_col - 2 || j < cut_col + 2)) {
     for (int p = 0; p < 12; p++) {
					for (int q = 0; q < 16; q++) {
     if (cells[i*m_ColCount + j][p * 16 + q] == 1) {
     Renderer->DrawSprite(*cellPic, glm::vec2(j * 320 + (q * 20) +320 - cur_x, i * 240 + p * 20 + 240 - cur_y), glm::vec2(20, 20), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
     }
					}
     }
     }
     }
     }
     
     */
    
    
    /*
     Renderer->DrawSprite(glm::vec2(320, 240), glm::vec2(20, 20), 0.0f,
     glm::vec3(1.0f, 1.0f, 1.0f));
     */
    
}
