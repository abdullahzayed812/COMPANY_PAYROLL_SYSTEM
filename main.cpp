#include <iostream>
#include <string>
#include <vector>

class Payable {
 public:
  virtual double getAmountToPay() const = 0;  // Pure virtual function.
  virtual ~Payable() = default;               // Virtual destructor (tell compiler to use default destructor).
};

class StaffMember : public Payable {
 protected:
  std::string name;
  std::string address;

 public:
  virtual ~StaffMember() = default;  // Virtual destructor
};

class Employee : public StaffMember {
 private:
  int dayToPay;

 public:
  virtual ~Employee() = default;  // Virtual destructor
};

class HourlyEmployee : public Employee {
 private:
  int totalWorkingHours;
  double salaryPerHour;

 public:
  HourlyEmployee(int hours, double rate) : totalWorkingHours(hours), salaryPerHour(rate) {}

  virtual double getAmountToPay() const override {
    std::cout << "HourlyEmployee\n";
    return this->totalWorkingHours * this->salaryPerHour;
  }
};

class SalariedEmployee : public Employee {
 private:
  double monthlySalary;

 public:
  SalariedEmployee(double salary) : monthlySalary(salary) {}

  virtual double getAmountToPay() const override {
    std::cout << "SalariedEmployee\n";
    return this->monthlySalary;
  }
};

class CommissionSalariedEmployee : public SalariedEmployee {
 private:
  double commissionRate;
  double currentMonthSalary;

 public:
  CommissionSalariedEmployee(double salary, double commission, double currentSalary)
      : SalariedEmployee(salary), commissionRate(commission), currentMonthSalary(currentSalary) {}

  virtual double getAmountToPay() const override {
    std::cout << "CommissionSalariedEmployee\n";
    return SalariedEmployee::getAmountToPay() + (this->commissionRate * this->currentMonthSalary);
  }
};

class Volunteer : public StaffMember {
 private:
  int currentPayment;

 public:
  Volunteer(int payment) : currentPayment(payment) {}

  virtual double getAmountToPay() const override { return this->currentPayment; }
};

class InvoiceItem {
 private:
  std::string description;
  double price;
  int quantity;

 public:
  InvoiceItem(const std::string& description, double price, int quantity)
      : description(description), price(price), quantity(quantity) {}

  double getPrice() const { return this->price * this->quantity; }
};

class Book : public InvoiceItem {
 private:
  std::string author;

 public:
  Book(const std::string& description, double price, int quantity, const std::string& author)
      : InvoiceItem(description, price, quantity), author(author) {}
};

class Food : public InvoiceItem {
 private:
  std::string expirationDate;

 public:
  Food(const std::string& description, double price, int quantity, const std::string& expirationDate)
      : InvoiceItem(description, price, quantity), expirationDate(expirationDate) {}
};

class Invoice : public Payable {
 private:
  int invoiceId = -1;
  std::vector<InvoiceItem*> invoiceItems;

 public:
  void addInvoiceItem(InvoiceItem* invoiceItem) { invoiceItems.push_back(invoiceItem); }

  virtual double getAmountToPay() const override {
    std::cout << "Invoice\n";

    double sum = 0;

    for (const InvoiceItem* invoiceItem : this->invoiceItems) {
      sum += invoiceItem->getPrice();
    }

    return sum;
  }

  virtual ~Invoice() {
    for (const InvoiceItem* invoiceItem : this->invoiceItems) {
      delete invoiceItem;
    }
  }
};

class Payroll {
 private:
  std::vector<Payable*> payables;

 public:
  void addPayable(Payable* payable) { this->payables.push_back(payable); }

  void pay() {
    std::cout << "Payroll::pay\n";
    for (const Payable* payable : this->payables) {
      payable->getAmountToPay();
    }
  }

  ~Payroll() {
    for (const Payable* payable : this->payables) {
      delete payable;
    }
  }
};

class Company {
 private:
  Payroll payroll;

 public:
  Company() {}

  void run() {
    // Adding payables with dynamic allocation
    payroll.addPayable(new Volunteer(1500));
    payroll.addPayable(new HourlyEmployee(160, 120));
    payroll.addPayable(new SalariedEmployee(2500));
    payroll.addPayable(new CommissionSalariedEmployee(2500, 0.3, 3000));

    // Creating and adding an invoice
    Invoice* invoice = new Invoice();
    invoice->addInvoiceItem(new Book("Introduction to electronics", 120.0, 3, "Hamza abdullah"));
    invoice->addInvoiceItem(new Food("Meal and rise", 1200, 3, "18-8-2025"));

    payroll.addPayable(invoice);
    payroll.pay();
  }
};

int main() {
  Company company;
  company.run();

  return 0;
}
