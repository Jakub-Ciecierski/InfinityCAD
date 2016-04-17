#ifndef ITEMID_H
#define ITEMID_H

#include <vector>

struct ItemID{
    std::vector<int> id_values;

    ItemID(std::vector<int>& id_values){
        this->id_values = id_values;
    }

    friend std::ostream& operator<<(std::ostream& os, const ItemID& item){
        os << "[";
        for(unsigned int i = 0;i < item.id_values.size(); i++) {
            os << item.id_values[i];
            if (i != item.id_values.size() - 1) {
                os << ", ";
            }
        }
        os << "]";

        return os;
    }

    bool operator==(const ItemID& itemID) const{
        for(unsigned int i = 0; i < id_values.size(); i++){
            if(itemID.id_values[i] != this->id_values[i])
                return false;
        }
        return true;
    }
};

#endif // ITEMID_H
