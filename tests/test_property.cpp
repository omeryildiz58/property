#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <musProperty.h>

class Counter {
    int count_ = 0;
    int setCalls_ = 0;

public:
    int& getCount() { return count_; }
    const int& getConstCount() const { return count_; }
    void setCount(const int& v) {
        count_ = v;
        ++setCalls_;
    }

    int getSetCalls() const { return setCalls_; }

    Property<&Counter::getCount, &Counter::setCount> count = this;
    Property<&Counter::getConstCount> readOnlyCount = this;
};

class MessageHolder {
    std::string text_;
    int copySets_ = 0;
    int moveSets_ = 0;

public:
    std::string& getText() { return text_; }
    void setText(const std::string& v) {
        text_ = v;
        ++copySets_;
    }
    void moveText(std::string&& v) {
        text_ = std::move(v);
        ++moveSets_;
    }

    int copyCount() const { return copySets_; }
    int moveCount() const { return moveSets_; }

    Property<&MessageHolder::getText, &MessageHolder::setText, &MessageHolder::moveText> text = this;
};

int main() {
    Counter c;
    c.count = 10;
    assert(c.count == 10);
    assert(c.readOnlyCount == 10);
    assert(c.getSetCalls() == 1);

    c.count += 5;
    assert(c.count == 15);
    assert((c.count + 5) == 20);
    assert((c.readOnlyCount + 3) == 18);

    MessageHolder holder;
    std::string initial = "Merhaba";
    holder.text = initial;
    assert(holder.copyCount() == 1);
    assert(holder.moveCount() == 0);
    assert(static_cast<std::string&>(holder.text) == "Merhaba");

    holder.text = std::string("YILDIZ-ERP");
    assert(holder.moveCount() == 1);
    assert(static_cast<std::string&>(holder.text) == "YILDIZ-ERP");

    std::cout << "test_property: all assertions passed!" << std::endl;
    return 0;
}
