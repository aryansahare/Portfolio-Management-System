#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm> 
#include <sstream>
#include <limits>

class Transaction
{
public:
    Transaction(int id, int assetId, double quantity, double priceAtTransaction, const std::string& transactionType, const std::string& date)
        : transactionID(id), assetID(assetId), quantity(quantity), priceAtTransaction(priceAtTransaction), transactionType(transactionType), date(date) {}

    std::string getDetails() const
    {
        std::ostringstream oss;
        oss << "ID: " << transactionID
            << ", Asset ID: " << assetID
            << ", Quantity: " << quantity
            << ", Price at Transaction: " << priceAtTransaction
            << ", Type: " << transactionType
            << ", Date: " << date;
        return oss.str();
    }

private:
    int transactionID;
    int assetID;
    double quantity;
    double priceAtTransaction;
    std::string transactionType;
    std::string date;
};

class Asset
{
public:
    Asset(int id, const std::string& name, double price)
        : id(id), name(name), price(price) {}

    int getId() const
    {
        return id;
    }

    const std::string& getName() const
    {
        return name;
    }

    double getPrice() const
    {
        return price;
    }

    void updatePrice(double newPrice)
    {
        price = newPrice;
    }

private:
    int id;
    std::string name;
    double price;
};

class Portfolio
{
public:
    Portfolio(int id, const std::string& name)
        : portfolioID(id), name(name) {}

    void addAsset(const Asset& asset)
    {
        assets.push_back(asset);
    }

    void removeAsset(int assetId)
    {
        auto it = std::remove_if(assets.begin(), assets.end(),
            [assetId](const Asset& asset) { return asset.getId() == assetId; });
        assets.erase(it, assets.end());
    }

    void addTransaction(const Transaction& transaction)
    {
        transactions.push_back(transaction);
    }

    double getAssetValue() const
    {
        double totalValue = 0.0;
        for (const auto& asset : assets)
        {
            totalValue += asset.getPrice();
        }
        return totalValue;
    }

    double getPerformance() const
    {
        // Placeholder implementation
        return getAssetValue();
    }

    int getId() const
    {
        return portfolioID;
    }

    const std::string& getName() const
    {
        return name;
    }

private:
    int portfolioID;
    std::string name;
    std::vector<Asset> assets;
    std::vector<Transaction> transactions;
};

class PortfolioManager
{
public:
    void createPortfolio(int id, const std::string& name)
    {
        portfolios.emplace_back(id, name);
    }

    void deletePortfolio(int id)
    {
        auto it = std::remove_if(portfolios.begin(), portfolios.end(),
            [id](const Portfolio& p) { return p.getId() == id; });
        portfolios.erase(it, portfolios.end());
    }

    Portfolio* getPortfolio(int id)
    {
        for (auto& portfolio : portfolios)
        {
            if (portfolio.getId() == id)
            {
                return &portfolio;
            }
        }
        return nullptr;
    }

    std::vector<Portfolio> listPortfolios() const
    {
        return portfolios;
    }

private:
    std::vector<Portfolio> portfolios;
};

class User
{
public:
    User(const std::string& username, const std::string& password, const std::string& email)
        : username(username), password(password), email(email) {}

    void setPassword(const std::string& pass)
    {
        password = pass;
    }

    bool checkPassword(const std::string& pass) const
    {
        return password == pass;
    }

    std::string getUsername() const
    {
        return username;
    }

    std::string getEmail() const
    {
        return email;
    }

private:
    std::string username;
    std::string password;
    std::string email;
};

class UserManager
{
public:
    bool registerUser(const std::string& username, const std::string& password, const std::string& email)
    {
        if (users.find(username) != users.end())
        {
            return false;
        }

        users.emplace(username, User(username, password, email));
        return true;
    }

    bool loginUser(const std::string& username, const std::string& password)
    {
        auto it = users.find(username);
        if (it != users.end())
        {
            return it->second.checkPassword(password);
        }
        return false;
    }

    User* getUser(const std::string& username)
    {
        auto it = users.find(username);
        if (it != users.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    bool deleteUser(const std::string& username)
    {
        auto it = users.find(username);
        if (it != users.end())
        {
            users.erase(it);
            return true;
        }
        return false;
    }

private:
    std::unordered_map<std::string, User> users;
};

class Session
{
public:
    Session() : currentUser(nullptr) {}

    bool login(const std::string& username, const std::string& password, UserManager& userManager)
    {
        User* user = userManager.getUser(username);
        if (user != nullptr && user->checkPassword(password))
        {
            currentUser = user;
            return true;
        }
        return false;
    }

    void logout()
    {
        currentUser = nullptr;
    }

    User* getCurrentUser() const
    {
        return currentUser;
    }

private:
    User* currentUser;
};



class UserInterface
{
public:
    UserInterface(PortfolioManager& pm, UserManager& um, Session& s)
        : portfolioManager(pm), userManager(um), session(s) {}

    void start()
    {
        while (true)
        {
            displayMainMenu();
            int choice;
            std::cin >> choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 4.\n";
                continue;
            }

            switch (choice)
            {
                case 1:
                    handleLogin();
                    break;
                case 2:
                    handleRegister();
                    break;
                case 3:
                    handlePortfolioManagement();
                    break;
                case 4:
                    std::cout << "Exiting...\n";
                    return;
                default:
                    std::cout << "Invalid input. Please choose a number between 1 and 4.\n";
                    break;
            }
        }
    }

private:
    void displayMainMenu()
    {
        std::cout << "\n===== Main Menu =====\n";
        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "3. Manage Portfolios\n";
        std::cout << "4. Exit\n";
        std::cout << "=====================\n";
        std::cout << "Please choose an option (1-4): ";
    }

    void handleLogin()
    {
        std::string username, password;
        std::cout << "\n--- Login Page ---\n";
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        if (session.login(username, password, userManager))
        {
            std::cout << "Login successful. Welcome, " << username << "!\n";
        }
        else
        {
            std::cout << "Login failed. Please check your username and password.\n";
        }
    }

    void handleRegister()
    {
        std::string username, email, password;
        std::cout << "\n--- Register Page ---\n";
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        std::cout << "Enter email: ";
        std::cin >> email;

        if (userManager.registerUser(username, password, email))
        {
            std::cout << "User registered successfully.\n";
        }
        else
        {
            std::cout << "Username already exists. Please choose a different username.\n";
        }
    }

    void handlePortfolioManagement()
    {
        if (!session.getCurrentUser())
        {
            std::cout << "You need to be logged in to manage portfolios.\n";
            return;
        }

        while (true)
        {
            displayPortfolioMenu();
            int choice;
            std::cin >> choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 5.\n";
                continue;
            }

            switch (choice)
            {
                case 1:
                    createPortfolio();
                    break;
                case 2:
                    selectAndManagePortfolio();
                    break;
                case 3:
                    listPortfolios();
                    break;
                case 4:
                    logoutUser();
                    return; // Return to the main menu
                case 5:
                    return; // Return to the main menu
                default:
                    std::cout << "Invalid input. Please choose a number between 1 and 5.\n";
                    break;
            }
        }
    }

    void displayPortfolioMenu()
    {
        std::cout << "\n===== Portfolio Management =====\n";
        std::cout << "1. Create Portfolio\n";
        std::cout << "2. Manage Portfolio\n";
        std::cout << "3. List Portfolios\n";
        std::cout << "4. Logout\n";
        std::cout << "5. Return to Main Menu\n";
        std::cout << "=================================\n";
        std::cout << "Please choose an option (1-5): ";
    }

    void createPortfolio()
    {
        int portfolioID;
        std::string name;
        std::cout << "\n--- Create Portfolio ---\n";
        std::cout << "Enter portfolio ID: ";
        std::cin >> portfolioID;
        std::cout << "Enter portfolio name: ";
        std::cin.ignore(); // To clear the newline character left in the buffer
        std::getline(std::cin, name);

        portfolioManager.createPortfolio(portfolioID, name);
        std::cout << "Portfolio created successfully.\n";
    }

    void listPortfolios()
    {
        std::vector<Portfolio> portfolios = portfolioManager.listPortfolios();
        std::cout << "\n--- List of Portfolios ---\n";
        for (const auto& portfolio : portfolios)
        {
            std::cout << "ID: " << portfolio.getId() << ", Name: " << portfolio.getName() << '\n';
        }
    }

    void selectAndManagePortfolio()
    {
        int portfolioID;
        std::cout << "\n--- Select Portfolio ---\n";
        std::cout << "Enter portfolio ID: ";
        std::cin >> portfolioID;

        Portfolio* portfolio = portfolioManager.getPortfolio(portfolioID);
        if (!portfolio)
        {
            std::cout << "Portfolio not found.\n";
            return;
        }

        while (true)
        {
            displayPortfolioActions();
            int choice;
            std::cin >> choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 4.\n";
                continue;
            }

            switch (choice)
            {
                case 1:
                    addAssetToPortfolio(*portfolio);
                    break;
                case 2:
                    removeAssetFromPortfolio(*portfolio);
                    break;
                case 3:
                    showPortfolioDetails(*portfolio);
                    break;
                case 4:
                    return; // Return to portfolio management menu
                default:
                    std::cout << "Invalid input. Please choose a number between 1 and 4.\n";
                    break;
            }
        }
    }

    void displayPortfolioActions()
    {
        std::cout << "\n===== Portfolio Actions =====\n";
        std::cout << "1. Add Asset\n";
        std::cout << "2. Remove Asset\n";
        std::cout << "3. Show Portfolio Details\n";
        std::cout << "4. Return to Portfolio Management Menu\n";
        std::cout << "============================= \n";
        std::cout << "Please choose an option (1-4): ";
    }

    void addAssetToPortfolio(Portfolio& portfolio)
    {
        int assetID;
        std::string assetName;
        double assetPrice;
        std::cout << "\n--- Add Asset to Portfolio ---\n";
        std::cout << "Enter asset ID: ";
        std::cin >> assetID;
        std::cout << "Enter asset name: ";
        std::cin.ignore(); // To clear the newline character left in the buffer
        std::getline(std::cin, assetName);
        std::cout << "Enter asset price: ";
        std::cin >> assetPrice;

        Asset newAsset(assetID, assetName, assetPrice);
        portfolio.addAsset(newAsset);
        std::cout << "Asset added successfully.\n";
    }

    void removeAssetFromPortfolio(Portfolio& portfolio)
    {
        int assetID;
        std::cout << "\n--- Remove Asset from Portfolio ---\n";
        std::cout << "Enter asset ID: ";
        std::cin >> assetID;

        portfolio.removeAsset(assetID);
        std::cout << "Asset removed successfully.\n";
    }

    void showPortfolioDetails(const Portfolio& portfolio)
    {
        std::cout << "\n--- Portfolio Details ---\n";
        std::cout << "Portfolio ID: " << portfolio.getId() << '\n';
        std::cout << "Portfolio Name: " << portfolio.getName() << '\n';
        std::cout << "Total Asset Value: $" << portfolio.getAssetValue() << '\n';
        // Additional details can be shown here, such as asset list and performance
    }

    void logoutUser()
    {
        session.logout();
        std::cout << "Logged out successfully.\n";
    }

    PortfolioManager& portfolioManager;
    UserManager& userManager;
    Session& session;
};


int main()
{
    // Initialize necessary components
    PortfolioManager portfolioManager;
    UserManager userManager;
    Session session;

    // Create an instance of UserInterface
    UserInterface ui(portfolioManager, userManager, session);

    // Start the user interface
    ui.start();

    return 0;
}
