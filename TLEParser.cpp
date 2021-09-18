#include "TLEParser.h"

vector<TLEdata> SatSearch(fs::path& satPath)
{
    vector<TLEdata> satelliteList;
    TLEdata satBuf;
    string line1, line2;
    string buf={};
    fs::path iter;
    if (fs::exists(satPath) && fs::is_directory(satPath)) //checks if the path is a directory and exists
    {
        for (const auto& entry : fs::directory_iterator(satPath)) //iterates objects in the directory
        {
            if (!fs::is_directory(entry.path())) //if the object isn't a directory it tries to open it with fstream
            {
                if(entry.path().extension()==".txt"){
                    iter = entry.path();
                    ifstream f(entry.path());
                    while (getline(f, satBuf.SatName))
                    {
                        getline(f,line1);
                        getline(f,line2);

                        //line1
                        satBuf.IntDesignator=line1.substr(9, 7);
                        satBuf.Epoch=line1.substr(18, 14);

                        //line2
                        satBuf.CatNo=stoi(line2.substr(2, 5));
                        satBuf.Inclination=stof(line2.substr(8, 8));
                        satBuf.RAAN=stof(line2.substr(17, 8));
                        buf.append("0.");
                        buf.append(line2.substr(26, 7));
                        satBuf.Eccentricity=stof(buf);
                        satBuf.ArgPerigee=stof(line2.substr(34, 8));
                        satBuf.MeanAnomaly=stof(line2.substr(43, 8));
                        satBuf.MeanMotion=stof(line2.substr(52, 11));

                        satBuf.origFile=entry.path().filename();

                        satelliteList.push_back(satBuf);
                        line1={};
                        buf={};
                        line2={};
                    }
                }
            }
        }
    }
    return satelliteList;
}
