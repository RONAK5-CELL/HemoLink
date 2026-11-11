#include <iostream>

#include "Donormanager.h"
#include "Inventory.h"
#include "HospitalManager.h"
#include "RequestManager.h"

using namespace std;

int main()
{
    DonorManager dm;
    InventoryManager im;
    HospitalManager hm;
    RequestManager rm;

    int choice;

    do
    {
        cout << "\n========================================";
        cout << "\n                 HEMOLINK";
        cout << "\n  Intelligent Blood Bank System";
        cout << "\n========================================";

        cout << "\n\n       DONOR MANAGEMENT";
        cout << "\n1. Register New Donor";
        cout << "\n2. Display All Donors";
        cout << "\n3. Search Donor";
        cout << "\n4. Update Donor";
        cout << "\n5. Delete Donor";

        cout << "\n\n       BLOOD INVENTORY";
        cout << "\n6. Add Blood Stock";
        cout << "\n7. Display Blood Stock";
        cout << "\n8. Search Blood Stock";

        cout << "\n\n       HOSPITAL MANAGEMENT";
        cout << "\n9. Register Hospital";
        cout << "\n10. Display Hospitals";
        cout << "\n11. Search Hospital";
        cout << "\n12. Update Hospital";
        cout << "\n13. Delete Hospital";

        cout << "\n\n       BLOOD REQUEST MANAGEMENT";
        cout << "\n14. Generate Blood Request";
        cout << "\n15. Display All Requests";
        cout << "\n16. Search Request";
        cout << "\n17. Update Request Status";
        cout << "\n18. Delete Request";

        cout << "\n\n19. Exit";

        cout << "\n\nEnter Your Choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                dm.addDonor();
                break;

            case 2:
                dm.displayDonors();
                break;

            case 3:
                dm.searchDonor();
                break;

            case 4:
                dm.updateDonor();
                break;

            case 5:
                dm.deleteDonor();
                break;

            case 6:
                im.addStock();
                break;

            case 7:
                im.displayStock();
                break;

            case 8:
                im.searchStock();
                break;

            case 9:
                hm.addHospital();
                break;

            case 10:
                hm.displayHospitals();
                break;

            case 11:
                hm.searchHospital();
                break;

            case 12:
                hm.updateHospital();
                break;

            case 13:
                hm.deleteHospital();
                break;

            case 14:
                rm.addRequest();
                break;

            case 15:
                rm.displayRequests();
                break;

            case 16:
                rm.searchRequest();
                break;

            case 17:
                rm.updateRequestStatus();
                break;

            case 18:
                rm.deleteRequest();
                break;

            case 19:
                cout << "\nThank you for using HemoLink!\n";
                break;

            default:
                cout << "\nInvalid Choice! Try Again.\n";
        }

    } while(choice != 19);

    return 0;
}

