/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * A8: Qt Sprite Editor
 *
 * VIDEO DEMO LINK:
 * https://www.youtube.com/watch?v=K606owc6uvY
 */
#include "model.h"

/*!
 * Model of sprite editing program. Handles what to do when creating an istance of the program. Handles
 * data inputs from controller and deciphers how to use data.
 */

/**
 * @brief Model::Model Constructor for our model class
 * @param parent
 */
Model::Model(QObject *parent) : QObject(parent)
{
    fps = 1;
    timer = new QTimer(this);
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
    // change preview label to reflect new main sprite
    updatePixmap();
    // call method that will handle thumbnail previews
    createNewThumbnailPreview();
}

/*!
 * \brief Model::getDimensionsAndInitializeFirstSprite slot method decides what to do given the dimensions of the sprite.
 * Will set this class' dimension member and and create and initialize the first sprite of this class
 * \param dim
 */
void Model::getDimensionsAndInitializeFirstSprite(int dim)
{
    spriteDimensions = dim;
    scale = spriteDimensions/previewSize;
    // create the first sprite of model
    createNewSprite();
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
    int xInPixelSpace =  transformCoordToXandYSpriteSpace(x);
    int yInPixelSpace =  transformCoordToXandYSpriteSpace(y);

    // if thickness is 3 set x and y such that they get drawn with middle most pixel being the source pixle with all others being drawn around it
    if(thickness==3){
        xInPixelSpace = xInPixelSpace-1;
        yInPixelSpace = yInPixelSpace-1;
    }

    // for loop goes through the thickness and adds pixels based on thickness
    // first for loop loops through y pixel clicked + thickness
    for(int yDependingOnThick = 0; yDependingOnThick<thickness; yDependingOnThick++)
        // second for loop does the same with x
        for(int xDependingOnThick = 0; xDependingOnThick<thickness;xDependingOnThick++ ){
            int currentXPixel = xInPixelSpace+xDependingOnThick;
            int currentYPixel = yInPixelSpace+yDependingOnThick;

            // check that pixel drawn is not out range
            if(currentYPixel >= 0 && currentXPixel >= 0 && currentXPixel<spriteDimensions && currentYPixel<spriteDimensions)
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
    int xInPixelSpace =  transformCoordToXandYSpriteSpace(x);
    int yInPixelSpace =  transformCoordToXandYSpriteSpace(y);

    // send scaled coords to view
    emit sendCoords("x: "+QString::number(xInPixelSpace) + "  y: " + (QString::number(yInPixelSpace)));
}

/*!
 * \brief Model::makeGrid makes the grid for the sprite drawing.
 * \param canvasSize size of the canvas/preview drawing of sprite
 */
void Model::makeGrid(int canvasSize){
    // add grid lines on top of sprite image's pixmap
    QPixmap pixmap(QPixmap::fromImage(sprite->getImage()).scaled(canvasSize, canvasSize, Qt::KeepAspectRatio));
    QPainter painter(&pixmap);
    // use black as the line color
    painter.setPen(QColor(0, 0, 0, 200));

    // draw vertical and horizontal lines
    for(float lineIndex = 0; lineIndex <= pixmap.width(); lineIndex+=pixmap.width()/spriteDimensions){
        //vertical lines
        painter.drawLine(lineIndex, 0, lineIndex, pixmap.height());
        //horizontal lines
        painter.drawLine(0, lineIndex, pixmap.width(), lineIndex);

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
void Model::createNewThumbnailPreview(){
    QLabel *temp = new QLabel();
    // set the the label of the temp to the image of the most recent preview of the sprite
    temp->setPixmap(QPixmap::fromImage(sprite->getImage().scaled(prefferedThumbnailSize, prefferedThumbnailSize, Qt::KeepAspectRatio)));
    emit sendThumbnailLabel(QPixmap::fromImage(sprite->getImage().scaled(prefferedThumbnailSize, prefferedThumbnailSize, Qt::KeepAspectRatio)), currentIndexOfSprites);
}

/*!
 * \brief Model::updateAndPaintALl slot takes current sprite and fills all of the image
 * with color given
 * \param selected color to set as the fill of the image
 */
void Model::updateAndPaintALl(QColor selected){
    sprite->currSprite.fill(selected);
    emit updateCurrentSpriteThumbnail(QPixmap::fromImage(sprite->getImage()).scaled(prefferedThumbnailSize, prefferedThumbnailSize, Qt::KeepAspectRatio), currentIndexOfSprites);
}

/*!
 * \brief Model::clearCurrentSprite Takes current sprite being previewed and sets the sprite to be blank
 */
void Model::clearCurrentSprite(){
    QColor blank(0,0,0,0);
    sprite->currSprite.fill(blank);
    emit updateCurrentSpriteThumbnail(QPixmap::fromImage(sprite->getImage()).scaled(prefferedThumbnailSize, prefferedThumbnailSize, Qt::KeepAspectRatio), currentIndexOfSprites);

    updatePixmap();
}

/*!
 * \brief Model::write MEthod writes to QJson object and allows for saving to files
 * \param json json object being written to
 */
void Model::write(QJsonObject &json) const
{
    int currIndex = 0;

    // initialize json member variables to those of this class
    json["height"] = spriteDimensions;
    json["width"] = spriteDimensions;
    json["numberOfFrames"] = sprites.length();

    // go through sprites and convert all x and y and rgb values to arrays and make an array of arrays
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
                colors.append(s->getPixel(j, i).red());
                colors.append(s->getPixel(j, i).green());
                colors.append(s->getPixel(j, i).blue());
                colors.append(s->getPixel(j, i).alpha());
                currRow.append(colors);
            }
            currSprite.append(currRow);
        }
        allSprites.insert(QString("frame%1").arg(currIndex), currSprite);

        currIndex++;
    }
    json["frames"] = allSprites;
}

/*!
 * \brief Model::read MEthod reads sprite information from json object. Used to load sprite frames
 * \param json json being read from
 */
void Model::read(QJsonObject &json){
    timer->stop();
    int height = -1;
    int width = -1;
    int frames = -1;
    QJsonObject test;

    // get height if exists
    if (json.contains("height") && json["height"].isDouble()){
        height = json["height"].toInt();
    }

    // get width if exists
    if (json.contains("width") && json["width"].isDouble()){
        width = json["width"].toInt();
    }

    // check if height and width does not equal
    if (height != width){
        // tell user about error
        emit errorWhenParsingJsonFile();
        timer->start(1000/fps);
        return;
    }
    // json is ready to be read, clear all preexisting layers
    emit clearButtonThumbnails();

    // get numberOfFrames if exists
    if (json.contains("numberOfFrames") && json["numberOfFrames"].isDouble()){
        frames = json["numberOfFrames"].toInt();
    }

    // if json contains frames get it
    if(json.contains("frames"))
    {
        test = json["frames"].toObject();
    }


    currentAnimatedSpriteIndex = 0;
    this->getDimensionsAndInitializeFirstSprite(height);
    sprites.clear();


    for(int i = 0; i < frames; i ++)
    {
        QJsonArray currentFrame = test[QString("frame%1").arg(i)].toArray();
        sprite = new Sprite(height);
        for(int row = 0; row < height; row++){
            QJsonArray rowArray = currentFrame.at(row).toArray();
            for(int col = 0; col < width; col++){
                QJsonArray colorArray = rowArray.at(col).toArray();
                int red = colorArray[0].toInt();
                int blue = colorArray[1].toInt();
                int green = colorArray[2].toInt();
                int alpha = colorArray[3].toInt();
                sprite->setPixel(row, col, QColor(red,blue,green,alpha));
            }
        }
        // update all thumbnails
        sprites.push_back(sprite);
        if(i!=0)
            emit sendThumbnailLabel(QPixmap::fromImage(sprite->getImage()).scaled(83, 83, Qt::KeepAspectRatio), i);

    }
    // reset sprite to be the first sprite
    sprite = sprites[0];
    currentIndexOfSprites = 0;
    // update current sprite's thumbnail which will be first
    emit updateCurrentSpriteThumbnail(QPixmap::fromImage(sprite->getImage()).scaled(83, 83, Qt::KeepAspectRatio), currentIndexOfSprites);
    timer->start(1000/fps);
    updatePixmap();
}

/*!
 * \brief Model::save method takes json that is written to and writes all json into a file
 * \param fileName name of file being written to
 */
void Model::save(QString fileName)
{

    write(jsonObj);
    QJsonDocument temp(jsonObj);
    QFile file(fileName);
    // open files and write to them the contents of json
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(temp.toJson(QJsonDocument::Compact));
    file.close();
}

/*!
 * \brief Model::open method that takes filename and reads all necessary data from it to create a new state in sprites
 * \param fileName file being read from
 */
void Model::open(QString fileName)
{

    QFile jsonFile(fileName);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonDocument JsonDocument = QJsonDocument::fromJson(jsonFile.readAll());
    jsonFile.close();
    // read from files the json object
    QJsonObject RootObject = JsonDocument.object();
    read(RootObject);
}

/*!
 * \brief Model::changeSpriteToIndex takes given index and sets current sprite to that one in list
 * \param index index of sprite to change to
 */
void Model::changeSpriteToIndex(int index){
    // get whatever sprite is at the index and update method variables accordingly
    currentIndexOfSprites = index;
    sprite = sprites[index];
    updatePixmap();
}

/*!
 * \brief Model::paintSprite paint bucket's sprite at given relative x and y location of canvas widget. Translates
 * x and y to pixel space coords and modifies sprite to paint where is selected
 * \param x canvas-relative (0-513) x value
 * \param y canvas-relative (0-513) y value
 * \param color color being painted
 */
void Model::paintSprite(int x, int y, const QColor& color)
{
    int xInPixelSpace =  transformCoordToXandYSpriteSpace(x);
    int yInPixelSpace =  transformCoordToXandYSpriteSpace(y);

    // check if color of paint bucket pixel is not same, if same do not bucket, if isnt paint bucket
    if(sprite->getPixel(xInPixelSpace, yInPixelSpace) != color)
    {
        // update by calling sprite's paint bucket and emit to update view thumbnails
        sprite->paintBucket(xInPixelSpace, yInPixelSpace, color, sprite->getPixel(xInPixelSpace, yInPixelSpace));
        emit updateCurrentSpriteThumbnail(QPixmap::fromImage(sprite->getImage()).scaled(prefferedThumbnailSize, prefferedThumbnailSize, Qt::KeepAspectRatio), currentIndexOfSprites);
    }
}

/*!
 * \brief Model::transformCoordToXandYSpace method takes relative location on canvas (0 to 513) and turns it to pixel
 * based locations (0 to 16, 32, etc)
 * \param xOrY single coordinate to transform
 * \return location of x or y relative to sprite
 */
int Model::transformCoordToXandYSpriteSpace(int xOrY){
    return xOrY*scale;
}
