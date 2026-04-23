/**
 * MAIN.CPP - Multi-Branch Banking Management System
 * 
 * Author: Trendt
 * Student ID: 402310946
 * Date: Current Date
 * 
 * Description: Complete banking system with:
 * - Teller authentication
 * - Customer PIN-based login
 * - Multiple account types (Savings, Cheque, Fixed Deposit, Student)
 * - Full transaction processing (Deposit, Withdraw, Transfer)
 * - Multi-branch support
 * - Interest calculation
 * - Search functionality
 * - Reports generation
 * - CSV data export
 * 
 * Compilation: g++ -std=c++11 main.cpp branch.cpp teller.cpp account.cpp 
 *              transaction.cpp reports.cpp utils.cpp -o banking_system
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "branch.h"
#include "teller.h"
#include "account.h"
#include "transaction.h"
#include "reports.h"
#include "utils.h"

using namespace std;

// Global branches vector - accessible across all functions
vector<Branch> branches;

/**
 * Main Program Entry Point
 * Displays main menu and handles user selection
 * Manages teller and customer sessions
 * @return 0 on successful execution
 */
int main() {
    // ============================================
    // INITIALIZATION - LOAD BRANCH DATA
    // ============================================
    
    // Check if branches.dat exists
    ifstream test("branches.dat");
    if (!test.good()) {
        // No existing data - create default branches
        cout << "No branches.dat found. Creating default branches...\n";
        init_default_branches(branches);
        save_branches(branches);
    }
    test.close();
    
    // Load branch data from file
    load_branches(branches);
    
    // ============================================
    // MAIN PROGRAM LOOP
    // ============================================
    
    bool run = true;
    
    while (run) {
        int choice;
        
        // Display Main Menu
        cout << "\n========================================\n";
        cout << "   MULTI-BRANCH BANKING SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Create Teller Account\n";
        cout << "2. Login As Teller\n";
        cout << "3. Login As Customer\n";
        cout << "4. Export Data to CSV\n";
        cout << "5. Exit\n";
        cout << "========================================\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        // ============================================
        // OPTION 1: CREATE TELLER ACCOUNT
        // ============================================
        
        if (choice == 1) {
            cout << "\n--- Create New Teller Account ---\n";
            create_teller_account();
        }
        
        // ============================================
        // OPTION 2: TELLER LOGIN & SESSION
        // ============================================
        
        else if (choice == 2) {
            string branch_code;
            
            if (login_teller(branch_code)) {
                // Teller login successful - enter teller session
                bool teller_session = true;
                
                while (teller_session) {
                    int choices;
                    
                    // Display Teller Menu
                    cout << "\n========== TELLER MENU ==========\n";
                    cout << "1.  Register New Customer\n";
                    cout << "2.  Deposit For Customer\n";
                    cout << "3.  Withdraw For Customer\n";
                    cout << "4.  Transfer For Customer\n";
                    cout << "5.  View All Branches\n";
                    cout << "6.  View Branch Details\n";
                    cout << "7.  Compare Branches\n";
                    cout << "8.  Apply Interest\n";
                    cout << "9.  Search For Customer\n";
                    cout << "10. Reports Menu\n";
                    cout << "11. Export Data\n";
                    cout << "12. Logout\n";
                    cout << "================================\n";
                    cout << "Enter choice: ";
                    cin >> choices;
                    
                    // Option 1: Register Customer
                    if (choices == 1) {
                        cout << "\n--- Register New Customer ---\n";
                        register_cust(branch_code);
                    }
                    
                    // Option 2: Customer Deposit
                    else if (choices == 2) {
                        string acc_no, pin;
                        cout << "\n--- Process Deposit ---\n";
                        cout << "Enter Account Number: ";
                        cin >> acc_no;
                        cout << "Enter Customer PIN: ";
                        cin >> pin;
                        
                        if (verify_pin(acc_no, pin)) {
                            deposit(acc_no);
                        } else {
                            cout << "Invalid PIN! Transaction cancelled.\n";
                        }
                    }
                    
                    // Option 3: Customer Withdrawal
                    else if (choices == 3) {
                        string acc_no, pin;
                        cout << "\n--- Process Withdrawal ---\n";
                        cout << "Enter Account Number: ";
                        cin >> acc_no;
                        cout << "Enter Customer PIN: ";
                        cin >> pin;
                        
                        if (verify_pin(acc_no, pin)) {
                            withdraw(acc_no);
                        } else {
                            cout << "Invalid PIN! Transaction cancelled.\n";
                        }
                    }
                    
                    // Option 4: Fund Transfer
                    else if (choices == 4) {
                        string acc_no, pin;
                        cout << "\n--- Process Transfer ---\n";
                        cout << "Enter Sender Account Number: ";
                        cin >> acc_no;
                        cout << "Enter Customer PIN: ";
                        cin >> pin;
                        
                        if (verify_pin(acc_no, pin)) {
                            transfer(acc_no);
                        } else {
                            cout << "Invalid PIN! Transaction cancelled.\n";
                        }
                    }
                    
                    // Option 5: View All Branches
                    else if (choices == 5) {
                        view_all_branches(branches);
                    }
                    
                    // Option 6: View Branch Details
                    else if (choices == 6) {
                        view_branch_details(branches);
                    }
                    
                    // Option 7: Compare Branches
                    else if (choices == 7) {
                        compare_branches();
                    }
                    
                    // Option 8: Apply Interest
                    else if (choices == 8) {
                        cout << "\n--- Applying Interest to All Accounts ---\n";
                        apply_interest();
                    }
                    
                    // Option 9: Search Customer
                    else if (choices == 9) {
                        int search_choice;
                        cout << "\n--- Search Customer ---\n";
                        cout << "1. Search by Account Number\n";
                        cout << "2. Search by Name\n";
                        cout << "3. Filter by Branch\n";
                        cout << "Enter choice: ";
                        cin >> search_choice;
                        
                        // Note: Add your search function calls here
                        // search_by_account();
                        // search_by_name();
                        // filter_by_branch();
                        cout << "Search feature - add your implementation\n";
                    }
                    
                    // Option 10: Reports Menu
                    else if (choices == 10) {
                        reports_menu();
                    }
                    
                    // Option 11: Export Data
                    else if (choices == 11) {
                        export_all_to_csv();
                    }
                    
                    // Option 12: Logout
                    else if (choices == 12) {
                        teller_session = false;
                        cout << "Logging out...\n";
                    }
                    
                    else {
                        cout << "Invalid option! Please try again.\n";
                    }
                }
            }
        }
        
        // ============================================
        // OPTION 3: CUSTOMER LOGIN & SESSION
        // ============================================
        
        else if (choice == 3) {
            string acc_no;
            
            if (login_customer(acc_no)) {
                // Customer login successful - enter customer session
                bool session = true;
                
                while (session) {
                    int opt;
                    
                    // Display Customer Menu
                    cout << "\n========= CUSTOMER MENU =========\n";
                    cout << "1. View Balance\n";
                    cout << "2. Deposit\n";
                    cout << "3. Withdraw\n";
                    cout << "4. Transfer\n";
                    cout << "5. View Statement\n";
                    cout << "6. Change PIN\n";
                    cout << "7. Logout\n";
                    cout << "================================\n";
                    cout << "Enter choice: ";
                    cin >> opt;
                    
                    // Option 1: View Balance
                    if (opt == 1) {
                        string pin;
                        cout << "Enter PIN: ";
                        cin >> pin;
                        view_balance(acc_no, pin);
                    }
                    
                    // Option 2: Deposit
                    else if (opt == 2) {
                        string pin;
                        cout << "Enter PIN: ";
                        cin >> pin;
                        if (verify_pin(acc_no, pin)) {
                            deposit(acc_no);
                        } else {
                            cout << "Invalid PIN!\n";
                        }
                    }
                    
                    // Option 3: Withdraw
                    else if (opt == 3) {
                        string pin;
                        cout << "Enter PIN: ";
                        cin >> pin;
                        if (verify_pin(acc_no, pin)) {
                            withdraw(acc_no);
                        } else {
                            cout << "Invalid PIN!\n";
                        }
                    }
                    
                    // Option 4: Transfer
                    else if (opt == 4) {
                        string pin;
                        cout << "Enter PIN: ";
                        cin >> pin;
                        if (verify_pin(acc_no, pin)) {
                            transfer(acc_no);
                        } else {
                            cout << "Invalid PIN!\n";
                        }
                    }
                    
                    // Option 5: View Statement
                    else if (opt == 5) {
                        view_statement(acc_no);
                    }
                    
                    // Option 6: Change PIN
                    else if (opt == 6) {
                        string old_pin, new_pin;
                        cout << "Enter current PIN: ";
                        cin >> old_pin;
                        cout << "Enter new PIN (5 digits): ";
                        cin >> new_pin;
                        change_pin(acc_no, old_pin, new_pin);
                    }
                    
                    // Option 7: Logout
                    else if (opt == 7) {
                        session = false;
                        cout << "Logging out...\n";
                    }
                    
                    else {
                        cout << "Invalid option!\n";
                    }
                }
            }
        }
        
        // ============================================
        // OPTION 4: EXPORT DATA TO CSV
        // ============================================
        
        else if (choice == 4) {
            cout << "\n--- Exporting Data to CSV ---\n";
            export_all_to_csv();
        }
        
        // ============================================
        // OPTION 5: EXIT PROGRAM
        // ============================================
        
        else if (choice == 5) {
            run = false;
            cout << "\nThank you for using the Banking System!\n";
            cout << "Goodbye!\n";
        }
        
        else {
            cout << "Invalid choice! Please enter 1-5.\n";
        }
    }
    
    return 0;  // Program terminated successfully
}

/*** END OF PROGRAM ***/