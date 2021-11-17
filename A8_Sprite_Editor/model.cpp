/*
 * Jimmy Trinh && Jacob Day
 * Software Practice II, CS 3505
 * A6: Qt Simon Game
 */
#include "model.h"
#include <iostream>

/*!
 * Model of sprite editing program. Handles what to do when creating an istance of the program. Handles
 * data inputs from controller and deciphers how to use data.
 */
using namespace std;
/**
 * @brief Model::Model Constructor for our model class
 * @param parent
 */
Model::Model(QObject *parent) : QObject(parent)
{
    fps = 1;

    timer = new QTimer(this); // asldkfjasldfkjasldfkjasldfkjsd
    connect(timer, &QTimer::timeout, this, &Model::runAnimation);
    timer->start(1000);
}

/*!
 * \brief Model::createNewSprite Method creates a new sprite and adds to list of
 * sprites.
 */
void Model::createNewSprite(){
    Sprite *temp = new Sprite(spriteDimensions);
    sprites.push_back(temp);
    sprite = temp;
    currentIndexOfSprites = sprites.length()-1;
    updatePixmap();
    // call method that will handle thumbnail previews
    setListPreview();
}

/*!
 * \brief Model::getDimensions slot method decides what to do given the dimensions of the sprite.
 * Will set this class' dimension member and and create and initialize the first sprite of this class
 * \param dim
 */
void Model::getDimensions(int dim)
{
    spriteDimensions = dim;
    scale = spriteDimensions/previewSize;
    // create the first sprite of model
    sprite = new Sprite(dim);
    // index of sprite is that of the only sprite we have
    currentIndexOfSprites = 0;
    sprites.push_back(sprite);

    setListPreview();
    //calls JSON write method.
//    write(jsonObj);
}

/*!
 * \brief Model::updatePixmap Wrapper method for updating pixmap by adding grid
 */
void Model::updatePixmap(){
    makeGrid(previewSize);
}

/*!
 * \brief Model::updateSprite Slot that will take given location and color and will set current
 * sprite's pixel at x and y to color given
 * \param x non-scaled x position of the grid (ranges from 0 - to preview size: 513
 * \param y non-scaled y position of the grid (ranges from 0 - to preview size: 513
 * \param color color setting sprite pixel to
 */
void Model::updateSprite(double x, double y, QColor color, int thickness)
{
    // convert 0 -513 x, y to 0 - 3, 15, 31, 63, ... x, y pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;
    // for loop goes through the thickness and adds pixels based on thickness
        // first for loop loops through y pixel clicked + thickness
            // second for loop does the same with x
    for(int yDependingOnThick = 0; yDependingOnThick<thickness; yDependingOnThick++)
        for(int xDependingOnThick = 0; xDependingOnThick<thickness;xDependingOnThick++ ){
            int currentXPixel = xInPixelSpace+xDependingOnThick;
            int currentYPixel = yInPixelSpace+yDependingOnThick;
            // check that pixel drawn is not out range
            if(currentXPixel<spriteDimensions && currentYPixel<spriteDimensions)
                sprite->setPixel(currentXPixel,currentYPixel,color);
        } 
    emit updateCurrentSpriteThumbnail(QPixmap::fromImage(sprite->getImage()).scaled(83, 83, Qt::KeepAspectRatio), currentIndexOfSprites);
}

/*!
 * \brief Model::getCoords slot that takes given coordinates, converts them to sprite based x, y
 * and sends them to update in view
 * \param x non-scaled x position of the grid (ranges from 0 - to preview size: 513
 * \param y non-scaled y position of the grid (ranges from 0 - to preview size: 513
 */
void Model::getCoords(double x, double y){

    // 0 - 3, 15, 31, 63 .... pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;
    // send scaled coords to view
    emit sendCoords("x: "+QString::number(xInPixelSpace) + "  y: " + (QString::number(yInPixelSpace)));
}


/*!
 * \brief Model::makeGrid makes the grid for the sprite drawing.
 * \param canvasSize size of the canvas/preview drawing of sprite
 */
void Model::makeGrid(int canvasSize){
    QPixmap pixmap(QPixmap::fromImage(sprite->getImage()).scaled(canvasSize, canvasSize, Qt::KeepAspectRatio));
    QPainter painter(&pixmap);

    painter.setPen(QColor(0, 0, 0, 200));

    //vertical lines
    for(float x = 0; x <= pixmap.width(); x+=pixmap.width()/spriteDimensions){
        painter.drawLine(x, 0, x, pixmap.height());
    }

    //horizontal lines
    for(float y = 0; y <= pixmap.height(); y+=pixmap.height()/spriteDimensions){
        painter.drawLine(0, y, pixmap.width(), y);
    }

    // send grid to view to draw
    emit sendGrid(pixmap);
}

/*!
 * \brief Model::setFps slot handles being sent the fps, will change the timer that handles the
 * animation speed
 * \param i new fps value
 */
void Model::setFps(int updatedFps){
    fps = updatedFps;
    timer->start(1000/fps);
}

/*!
 * \brief Model::runAnimation slot that handles beginning the animation cycle
 */
void Model::runAnimation(){
    previewAnimation();
    sendIndexedSprite();

}

/*!
 * \brief Model::sendIndexedSprite Will send sprites in sprites list one by one, method is called again and again
 * as this shall be running at all times. Handles sprite image sending loop
 */
void Model::sendIndexedSprite(){
    // only send if their are sprites right now in list
    if(sprites.length() != 0)
        // send the sprite at which the class member index variable is at as of rn
        emit sendAnimationPreviewPixmap(QPixmap::fromImage(sprites[currentAnimatedSpriteIndex++]->currSprite).scaled(128, 128, Qt::KeepAspectRatio));
    // reset the index variable
    if(currentAnimatedSpriteIndex>=sprites.length())
        currentAnimatedSpriteIndex = 0;
}

/*!
 * \brief Model::previewAnimation Will send the pixmaps to the view for the preview at actual size without scale
 */
void Model::previewAnimation(){
    if(sprites.length() != 0)
        // updating the preview by sending the current pixmap from the list of sprites
        emit sendPreviewPixmap(QPixmap::fromImage(sprites[currentAnimatedSpriteIndex]->currSprite));
}

/*!
 * \brief Model::setListPreview method takes the current sprite being worked on and send that sprites image
 * to the view to be shown
 */
void Model::setListPreview(){
    QLabel *temp = new QLabel();
    // set the the label of the temp to the image of the most recent preview of the sprite
    temp->setPixmap(QPixmap::fromImage(sprite->getImage().scaled(83, 83, Qt::KeepAspectRatio)));
    emit sendThumbnailLabel(QPixmap::fromImage(sprite->getImage().scaled(83, 83, Qt::KeepAspectRatio)));
}

/*!
 * \brief Model::updateAndPaintALl slot takes current sprite and fills all of the image
 * with color given
 * \param selected color to set as the fill of the image
 */
void Model::updateAndPaintALl(QColor selected){
    sprite->currSprite.fill(selected);
    emit updateCurrentSpriteThumbnail(QPixmap::fromImage(sprite->getImage()).scaled(83, 83, Qt::KeepAspectRatio), currentIndexOfSprites);

}

void Model::clearingSprite(){
    QColor blank(0,0,0,0);
    sprite->currSprite.fill(blank);
    emit updateCurrentSpriteThumbnail(QPixmap::fromImage(sprite->getImage()).scaled(83, 83, Qt::KeepAspectRatio), currentIndexOfSprites);

    updatePixmap();
}

void Model::write(QJsonObject &json) const
{
    int currIndex = 0;
    json["height"] = spriteDimensions;
    json["width"] = spriteDimensions;
    json["numberOfFrames"] = sprites.length();
    QJsonObject allSprites;
    for(Sprite* s : sprites)
    {
        QJsonArray currSprite;
        for(int j = 0; j < spriteDimensions; j ++)
        {
            QJsonArray currRow;
            for(int i = 0; i < spriteDimensions; i ++)
            {
                QJsonArray colors;
                colors.append(sprites[currIndex]->getPixel(j, i).red());
                colors.append(sprites[currIndex]->getPixel(j, i).green());
                colors.append(sprites[currIndex]->getPixel(j, i).blue());
                colors.append(sprites[currIndex]->getPixel(j, i).alpha());
                currRow.append(colors);
            }
            currSprite.append(currRow);
        }
        allSprites.insert(QString("frame%1").arg(currIndex), currSprite);

        currIndex++;

    }
    json["frames"] = allSprites;
//    QJsonDocument temp(json);
//    QFile file("C:\\Users\\dayja\\OneDrive\\Desktop\\temp.txt");
//    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
//    file.write(temp.toJson());
//    file.close();


}
void Model::read(QJsonObject &json){
    cout<< "we are reading rainbow"<< endl;
        int height;
        int width;
        int frames;
        if (json.contains("height") && json["height"].isDouble())
            height = json["height"].toInt();
            cout << "height is: "<< height << endl;

        if (json.contains("width") && json["width"].isDouble())
            width = json["width"].toInt();
            cout << "width is: "<< width << endl;

        if (height != width){
            return; // error message
        }

        if (json.contains("numberOfFrames") && json["numberOfFrames"].isDouble())
            frames = json["numberOfFrames"].toInt();
            cout << "frames is: "<< frames << endl;

        // PSEUDO CODE
        // Need to make actual code that can go through array.
        // Might need to go through QPixMap's matrix and assign each pixel an element from the "frames" array.
        for (int i = 0; i < frames; i++){
            cout << "frame" + QString::number(i).toStdString()<< endl;
            if (json.contains("frame"))
                cout << "frame is in the json: " << endl;
        }
}

void Model::save(QString fileName)
{
    write(jsonObj);
    QJsonDocument temp(jsonObj);
    QFile file(fileName);

    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(temp.toJson(QJsonDocument::Compact));
    file.close();
}

void Model::open(QString fileName)
{
    // NEED prompt for saving before opening a new sprite file
    QFile jsonFile(fileName);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonDocument JsonDocument = QJsonDocument::fromJson(jsonFile.readAll());
    jsonFile.close();

    QJsonObject RootObject = JsonDocument.object();
    read(RootObject);
}

/*!
 * \brief Model::changeSpriteToIndex takes given index and sets current sprite to that one in list
 * \param index index of sprite to change to
 */
void Model::changeSpriteToIndex(int index){
    currentIndexOfSprites = index;
    sprite = sprites[index];
    updatePixmap();
}

