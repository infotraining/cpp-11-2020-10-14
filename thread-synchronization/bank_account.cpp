#include <iostream>
#include <thread>

class BankAccount
{
    const int id_;
    double balance_;

public:
    BankAccount(int id, double balance)
        : id_(id)
        , balance_(balance)
    {
    }

    void print() const
    {
        std::cout << "Bank Account #" << id_ << "; Balance = " << balance_ << std::endl;
    }

    void transfer(BankAccount& to, double amount)
    {
        balance_ -= amount;
        to.balance_ += amount;
    }

    void withdraw(double amount)
    {
        balance_ -= amount;
    }

    void deposit(double amount)
    {
        balance_ += amount;
    }

    int id() const
    {
        return id_;
    }

    double balance() const
    {
        return balance_;
    }
};

void make_withdraws(BankAccount& ba, int no_of_operations)
{
    for (int i = 0; i < no_of_operations; ++i)
        ba.withdraw(1.0);
}

void make_deposits(BankAccount& ba, int no_of_operations)
{
    for (int i = 0; i < no_of_operations; ++i)
        ba.deposit(1.0);
}

void make_transfers(BankAccount& from, BankAccount& to, int no_of_operations, int thd_id)
{
    for (int i = 0; i < no_of_operations; ++i)
    {
        std::cout << "THD#" << thd_id << " transfer from ba#" << from.id()
                  << " to ba#" << to.id() << std::endl;

        from.transfer(to, 1.0);
    }
}


int main()
{
    const int no_of_iterations = 10'000;

    BankAccount ba1(1, 10'000);
    BankAccount ba2(2, 10'000);

    std::thread thd1(&make_withdraws, std::ref(ba1), no_of_iterations);
    std::thread thd2(&make_deposits, std::ref(ba1), no_of_iterations);

    thd1.join();
    thd2.join();

    std::cout << "After threads:\n";
    ba1.print();
    ba2.print();

    // std::cout << "\nTransfer:" << std::endl;

    // std::thread thd3(&make_transfers, std::ref(ba1), std::ref(ba2), no_of_iterations, 1);
    // std::thread thd4(&make_transfers, std::ref(ba2), std::ref(ba1), no_of_iterations, 2);

    // thd3.join();
    // thd4.join();

    // ba1.print();
    // ba2.print();
}
