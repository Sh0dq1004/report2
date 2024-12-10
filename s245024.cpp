/*学生番号, 名前, 完成した課題をここに記入*/
/*s245024, 北室頌太, 1,2,3*/

 #include <iostream>
 #include <vector>
 #include <fstream>
 #include <sstream>
 

struct stdat{//メンバを追加してもよい
    int id;
    std::string name;
    int math,eng,phys,chem;
    int total;
    char eval;
    std::string first,second,third,fourth,fifth;
    std::string Lab{"NotAssigned"};
    int RankLab{-1};

    int sum_up(){
        return math+eng+phys+chem;
    }
};

std::vector<std::string> Labnames
{"MathLab","StatisticsLab","ImageLab","ORLab","PhysicsLab","NotAssigned"};
std::vector<int> Cap {17,11,10,10,15}; //各研究室の定員
std::vector<int> BASE_SCORE {320,280,240,200}; //成績評価に使用
int Ashikiri {180}; //研究室割当ての足切り

// 研究室の割当てのための関数など, 必要ならば適宜定義せよ.

char evaluate(const stdat& s){
    std::vector<char> score_letter {'S','A','B','C'};
    for (int i{}; i<BASE_SCORE.size(); i++) if (s.total >= BASE_SCORE[i]) return score_letter[i];
    return 'F';
}

std::pair<std::string,int> lab_assignder(const stdat& s){
    if (s.total <= Ashikiri) return std::pair{"NotAssigned",-1};
    std::vector<std::string> will_list {s.first,s.second,s.third,s.fourth,s.fifth};
    for (int i{}; i<will_list.size();i++){
        int a{};
        for (; a<Labnames.size();a++){
            if (will_list[i]==Labnames[a]) break;
        }
        if (Cap[a] != 0) {
            Cap[a]-=1;
            return std::pair{Labnames[a],i+1};
        }
    }
    std::cout << "YOU MIGHT HAVE SOETHING WRONG...\n";
    return std::pair{"YOU MIGHT HAVE SOETHING WRONG...",404};
}

std::ostream& operator<<(std::ostream& out, const std::vector<std::string>& v){
    for (int i{}; i<v.size(); i++){
        out << v[i];
        if (i == v.size()-1) out << '\n';
        else out << ',';
    }
    return out;
}


int main(){
    std::ifstream fin1 {"score.csv"}, fin2 {"RankLabs.csv"};
    std::ofstream fout1 {"students.csv"}, fout2 {"Labmenbers.csv"};
    if(!fin1||!fin2){std::cerr<< "cannot open!\n"; return 1;}
    
    std::vector<stdat> vstdat(63); //学生63人分のデータ
    // 課題1.
    // fin1で成績データをvstdatに格納
    std::string line;
    std::getline(fin1,line); //ファイル1行目を読み飛ばす（２行目から各学生データ）
    /* 埋める */
    for (int i{}; i<vstdat.size(); i++){
        std::getline(fin1,line);
        std::istringstream iss(line);
        std::vector<std::string> datas;
        for (std::string s; std::getline(iss,s,',');) datas.push_back(s);
        vstdat[i].id=std::stoi(datas[0]);
        vstdat[i].name=datas[1];
        vstdat[i].math=std::stoi(datas[2]);
        vstdat[i].eng=std::stoi(datas[3]);
        vstdat[i].phys=std::stoi(datas[4]);
        vstdat[i].chem=std::stoi(datas[5]);
        vstdat[i].total=vstdat[i].sum_up();
        vstdat[i].eval=evaluate(vstdat[i]);
    }
    // fin2で研究室希望順位をvstdatに格納
    std::getline(fin2,line);
    /* 埋める */
    for (int i{}; i<vstdat.size(); i++){
        std::getline(fin2,line);
        std::istringstream iss(line);
        std::vector<std::string> datas;
        for (std::string s; std::getline(iss,s,',');) datas.push_back(s);
        vstdat[i].first=datas[2];
        vstdat[i].second=datas[3];
        vstdat[i].third=datas[4];
        vstdat[i].fourth=datas[5];
        vstdat[i].fifth=datas[6];
    }
    for (int i{}; i<vstdat.size(); i++){
        std::cout << vstdat[i].id << ',' 
        << vstdat[i].name << ',' 
        << vstdat[i].first << ',' 
        << vstdat[i].total << ',' 
        << vstdat[i].eval << '\n';
    }
    // 課題2,3
    fout1 << std::vector<std::string> {"id","name","total","eval","lab","rank"};
    for (int i{}; i<vstdat.size(); i++){
        auto [lab, rank] {lab_assignder(vstdat[i])};
        vstdat[i].Lab=lab;
        vstdat[i].RankLab=rank;
        fout1 << vstdat[i].id << ','
        << vstdat[i].name << ','
        << vstdat[i].total << ','
        << vstdat[i].eval << ','
        << vstdat[i].Lab << ','
        << vstdat[i].RankLab << std::endl;
    }
    
    std::vector<std::string> labmenbers{"MathLab","StatisticsLab","ImageLab","ORLab","PhysicsLab","NotAssigned"};
    for (int i{}; i<vstdat.size(); i++){
        for (int j{};j<Labnames.size();j++){
            if (vstdat[i].Lab==Labnames[j]){
                labmenbers[j]+=","+vstdat[i].name+"("+std::to_string(vstdat[i].RankLab)+")";
            }
        }
    }
    for (std::string s: labmenbers){
        fout2 << s << '\n';
    }


    return 0;}