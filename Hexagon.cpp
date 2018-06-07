#include "Hexagon.h"


/** @brief (one liner)
  *
  * (documentation goes here)
  */
void HexagonGrid::drawSVG(string fileName)
{
    svg::Document doc(fileName+".svg",svg::Layout(svg::Dimensions(gridWidth,gridHeight),svg::Layout::TopLeft));
    double marginL=0,marginConst=hexW*0.5;
    svg::Point hexCentre;
    double centreX=hexW*0.5;
    svg::Color hexColor=svg::Color::White;
    double hexX[6]={centreX-0.5*hexW,
                                    centreX-0.5*hexW,
                                    centreX,
                                    centreX+0.5*hexW,
                                    centreX+0.5*hexW,
                                    centreX};

    for(int i=0;i<dimension;i++){
        marginL=i*marginConst;
        double centreY =0.5*hexH+0.75* hexH* i;
        double hexY[6] = {centreY+0.25* hexH,
                            centreY-0.25* hexH,
                            centreY-0.5* hexH,
                            centreY-0.25* hexH,
                            centreY+0.25* hexH,
                            centreY+0.5* hexH};
        for(int j=0;j<dimension;j++)
        {
            hexColor=svg::Color::White;
            if (terminates.find(OffsetPoint(j,i))!=terminates.end())
                hexColor=svg::Color::Purple;
            else if (!path.empty())
                if(path.find(OffsetPoint(j,i))!=path.end())
                    hexColor=svg::Color::Yellow;
            doc<<(svg::Polygon(hexColor,svg::Stroke(.5,svg::Color::Black))
                  <<svg::Point(hexX[0]+marginL+hexW*j,hexY[0])
                  <<svg::Point(hexX[1]+marginL+hexW*j,hexY[1])
                  <<svg::Point(hexX[2]+marginL+hexW*j,hexY[2])
                  <<svg::Point(hexX[3]+marginL+hexW*j,hexY[3])
                  <<svg::Point(hexX[4]+marginL+hexW*j,hexY[4])
                  <<svg::Point(hexX[5]+marginL+hexW*j,hexY[5])
                  );
                  map<OffsetPoint, int>::iterator it = fronts.find(OffsetPoint(j,i));
                  doc<< svg::Text(svg::Point(hexX[0]+marginL+hexW*j+hexW*0.25, centreY+hexSize*0.25),  std::to_string(j)+","+std::to_string(i), svg::Color::Black, svg::Font(hexSize*0.65, "Arial"));
                  //if(it!=fronts.end())
                    //doc<< svg::Text(svg::Point(hexX[0]+marginL+hexW*j, centreY),  std::to_string(it->second), svg::Color::Black, svg::Font(hexSize*0.65, "Arial"));
        }
    }

    doc.save();
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
 HexagonGrid::~HexagonGrid()
{

}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
 HexagonGrid::HexagonGrid(int dimension, OffsetPoint start, OffsetPoint finish)
{
    this->start=start;
    this->finish=finish;
    this->dimension=dimension;
    hexSize=20;
    hexH=hexSize*2;
    hexW=hexSize*sqrt(3);
    gridHeight=hexH*(0.75*dimension+0.25);
    gridWidth=hexW*dimension+hexW*0.5*dimension;
    //BuildTerminates();
}
/** @brief (one liner)
  *
  * (documentation goes here)
  */
list<OffsetPoint> HexagonGrid::neighbours(int i, int j)
{
    list<OffsetPoint> res;
    if ((i>0)&&(j>=0)&&(j<dimension)&&(i<dimension) && terminates.find(OffsetPoint(j,i-1))==terminates.end())
    {
            res.push_back(OffsetPoint(j,i-1));
    }
    if ((j>0)&&(i>=0)&&(j<dimension)&&(i<dimension) && terminates.find(OffsetPoint(j-1,i))==terminates.end())
    {
        res.push_back(OffsetPoint(j-1,i));
    }
    if ((j>0)&&(i>=0)&&(i<dimension-1)&&(j<dimension) && terminates.find(OffsetPoint(j-1,i+1))==terminates.end())
    {
        res.push_back(OffsetPoint(j-1,i+1));
    }
    if ((j>=0)&&(i>=0)&&(i<dimension-1)&&(j<dimension) && terminates.find(OffsetPoint(j,i+1))==terminates.end()){
        res.push_back(OffsetPoint(j,i+1));
    }
    if ((j>=0)&&(i>=0)&&(i<dimension)&&(j<dimension-1) && terminates.find(OffsetPoint(j+1,i))==terminates.end()){
        res.push_back(OffsetPoint(j+1,i));
    }
    if ((j>=0)&&(i>0)&&(i<dimension)&&(j<dimension-1) && terminates.find(OffsetPoint(j+1,i-1))==terminates.end()){
        res.push_back(OffsetPoint(j+1,i-1));
    }
    return res;

}
/** @brief (one liner)
  *
  * (documentation goes here)
  */
void HexagonGrid::liAlg()
{
    fronts.insert(pair<OffsetPoint,int>(start, -1));
    list<OffsetPoint> *currentWave = new list<OffsetPoint>;
    currentWave->push_back(start);
    int currentWaveNum=0;
    while(fronts.find(finish) == fronts.end()){
        list<OffsetPoint> *newWave = new list<OffsetPoint>;
        for(list<OffsetPoint>::iterator it=currentWave->begin();it!=currentWave->end();it++)
        {
            list <OffsetPoint> tmpList = neighbours(it->y, it->x);
            for(list<OffsetPoint>::iterator neibIt = tmpList.begin();
                neibIt!=tmpList.end(); ++neibIt){
                    if (fronts.find(*neibIt)==fronts.end()){
                        fronts.insert(pair<OffsetPoint,int>(*neibIt, currentWaveNum));
                        newWave->push_back(*neibIt);
                    }
                }
        }
        drawSVG(std::to_string(currentWaveNum));
        currentWaveNum++;
        delete currentWave;
        currentWave = newWave;
        if(newWave->empty())
        {
            cout << "Can't find the path!\n";
            return;
        }
    }
    findPath();
    drawSVG("00_FINAL");
}
/** @brief (one liner)
  *
  * (documentation goes here)
  */
void HexagonGrid::findPath()
{
    if (fronts.find(finish)==fronts.end())
        return;
    OffsetPoint curElem=finish;
    path.insert(finish);
    int waveNum=fronts.find(finish)->second-1;

    while (curElem!=start){
        list <OffsetPoint> tmpList = neighbours(curElem.y, curElem.x);
        for(list<OffsetPoint>::iterator neibIt = tmpList.begin();
                neibIt!=tmpList.end(); ++neibIt)
        {
                    if(fronts.find(*neibIt) == fronts.end())
                        continue;
                    if ((fronts.find(*neibIt))->second==waveNum){
                        path.insert(*neibIt);
                        curElem=*neibIt;
                        waveNum--;
                        break;
                    }
                }
    }
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
void HexagonGrid::BuildTerminates()
{
    srand(time(NULL));
    int numOfWalls = rand()%(int)(dimension*dimension*0.7);
    for(int i=0; i<numOfWalls; i++)
    {
        OffsetPoint tmp(rand()%dimension-1, rand()%dimension-1);
        if(tmp == start || tmp == finish)
            continue;
        int len = rand()%(dimension);
        for(int j=0; j<len; j++)
        {
            list <OffsetPoint> tmpList = neighbours(tmp.y, tmp.x);
            if(tmpList.empty())
                break;
            int elemRand = rand()%tmpList.size()-1;
            int k=-1;
            for(list<OffsetPoint>::iterator neibIt = tmpList.begin();neibIt!=tmpList.end(); ++neibIt)
            {
                k++;
                if(k == elemRand && !(elemRand%2))
                {
                    if(OffsetPoint(neibIt->y, neibIt->x) == start || OffsetPoint(neibIt->y, neibIt->x) == finish)
                        break;
                    terminates.insert(OffsetPoint(neibIt->y, neibIt->x));
                    break;
                }

            }
        }
    }
}






