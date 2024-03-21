#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define V 19 

typedef struct {
    char busNumber[50];
    int availableSeats;
    char busType[20];
    int totalSeats;
} Bus;

typedef struct {
    char cityName[50];
    int numBuses;
    Bus buses[10];
} City;

int graph[V][V]; 
int parent[V]; 

const char *cityNames[V] = {"Bangalore", "Hubli", "Mumbai", "Kochi", "Chennai", "Madurai", "Gandhinagar", "Jaipur", "Delhi", "Lucknow", "Patna", "Kolkata", "Bhubaneshwar", "Vishakapatnam", "Nagpur", "Indore", "Hyderabad", "Kota", "Kanpur"};

City cities[V];

void addEdge(int u, int v, int weight) {
    graph[u - 1][v - 1] = weight;
    graph[v - 1][u - 1] = weight; 
}

void printGraph() {
    printf("Graph (Adjacency Matrix):\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
}

int findCityIndex(const char *city) {
    for (int i = 0; i < V; i++) {
        if (strcmp(city, cityNames[i]) == 0) {
            return i + 1;
        }
    }
    return -1;
}

int calculateTicketPrice(int totalWeight) {
    int ticketPrice;

    if (totalWeight <= 1000) {
        ticketPrice = totalWeight * 1;
    } else if (totalWeight > 1000 && totalWeight <= 2000) {
        ticketPrice = totalWeight * 0.85;
    } else if (totalWeight > 2000 && totalWeight <= 3000) {
        ticketPrice = totalWeight * 0.80;
    } else {
        ticketPrice = totalWeight * 0.75;
    }
    return ticketPrice;
}


int calculateTotalWeight(int route[], int routeSize) {
    int totalWeight = 0;
    for (int i = 0; i < routeSize - 1; i++) {
        totalWeight += graph[route[i]][route[i + 1]];
    }
    return totalWeight;
}

int minDistance(int dist[], int sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void printSolution(int dist[], int n, int destination, int source) {
    printf("\nShortest Path from %s to %s:\n", cityNames[source - 1], cityNames[destination - 1]);

    int path[V];
    int index = 0;

    for (int i = destination - 1; i != -1; i = parent[i]) {
        path[index++] = i;
    }

    for (int i = index - 1; i >= 0; i--) {
        printf("%s", cityNames[path[i]]);
        if (i > 0) {
            printf(" -> ");
        }
    }

    printf("\n");
}

void dijkstra(int src, int destination) {
    int dist[V];    
    int sptSet[V];   

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
        parent[i] = -1;
    }
    dist[src - 1] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);

        sptSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    printSolution(dist, V, destination,src);
}

void displayAndSaveTickets(char *startCity, char *endCity, int numtickets, const char *busNumber, int seatsBooked, int route[], int routeSize) {

    int totalWeight = calculateTotalWeight(route, routeSize);
    int ticketPrice = calculateTicketPrice(totalWeight);
    FILE *htmlFile = fopen("output.html", "w");

    if (htmlFile == NULL) {
        perror("Error opening output.html");
        exit(EXIT_FAILURE);
    }
    fprintf(htmlFile, "<html>\n<head>\n<title>Booked Tickets</title>\n");
    fprintf(htmlFile, "<style>\n");
    fprintf(htmlFile, "body { font-family: 'Arial', sans-serif; background-color: #FAFAFA; color: #121212; }\n"); // Adjust background and text color
    fprintf(htmlFile, ".heading1 { font-family: 'Arial'; font-size: 64px; }\n");
    fprintf(htmlFile, ".ticket { border: 2px solid #f00; padding: 20px; margin: 20px; background-color: #000; text-align: center; box-shadow: 0 0 10px rgba(255, 255, 255, 0.1); }\n");
    fprintf(htmlFile, ".ticket-header { font-size: 54px; font-weight: bold; margin-bottom: 10px; color: #fff; }\n");
    fprintf(htmlFile, ".ticket-info { font-size: 30px; margin-bottom: 10px; color: #fff; }\n");
    fprintf(htmlFile, ".route-info { font-size: 30px; margin-bottom: 10px; color: #fff; font-style: italic; }\n");
    fprintf(htmlFile, "</style>\n</head>\n<body>\n");
    
    // Write booked tickets to HTML
    fprintf(htmlFile, "<div class=\"heading1\">YOUR E-TICKET:</div>\n");
    fprintf(htmlFile, "<div class=\"ticket\">\n");
    fprintf(htmlFile, "<div class=\"ticket-info\"><strong>Start City:</strong> %s</div>\n", startCity);
    fprintf(htmlFile, "<div class=\"ticket-info\"><strong>End City:</strong> %s</div>\n", endCity);
    fprintf(htmlFile, "<div class=\"ticket-info\"><strong>Bus Number:</strong> %s</div>\n", busNumber);
    fprintf(htmlFile, "<div class=\"ticket-info\"><strong>Seats Booked:</strong> %d</div>\n", seatsBooked);
    fprintf(htmlFile, "<div class=\"ticket-info\"><strong>Ticket Price:</strong> Rs. %d</div>\n", ticketPrice);
    fprintf(htmlFile, "<div class=\"ticket-info\"><strong>Final Price:</strong> Rs. %d</div>\n", ticketPrice*seatsBooked);
    fprintf(htmlFile, "<div class=\"route-info\"><strong>Route:</strong> ");
    for (int i = routeSize - 1; i >= 0; i--) {
        fprintf(htmlFile, "%s", cityNames[route[i]]);
        if (i > 0) {
            fprintf(htmlFile, " &rarr; ");
        }
    }
    fprintf(htmlFile, "</div>\n");

    fprintf(htmlFile, "</div>\n");

    fprintf(htmlFile, "</body>\n</html>");

    fclose(htmlFile);
}

void displayAndConfirmBooking(int source, int destination, int numTickets) {
    int busIndex = -1;
    for (int i = 0; i < cities[source - 1].numBuses; i++) {
        if (cities[source - 1].buses[i].availableSeats >= numTickets) {
            busIndex = i;
            printf("Available Bus:\n");
            printf("Bus Number: %s\n", cities[source - 1].buses[i].busNumber);
            printf("Bus Type: %s\n", cities[source - 1].buses[i].busType);
            printf("Available Seats: %d\n", cities[source - 1].buses[i].availableSeats);

            char confirmation;
            printf("Do you want to confirm the booking? (y/n): ");
            scanf(" %c", &confirmation);

            if (confirmation == 'y' || confirmation == 'Y') {
                cities[source - 1].buses[busIndex].availableSeats -= numTickets;
                printf("Tickets booked from %s to %s on bus %s\n", cityNames[source - 1], cityNames[destination - 1], cities[source - 1].buses[busIndex].busNumber);
            } else {
                printf("Booking canceled.\n");
            }

            break;
        }
    }

    if (busIndex == -1) {
        printf("No available seats on any bus from %s to %s\n", cityNames[source - 1], cityNames[destination - 1]);
    }
}

int main() {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }
    strcpy(cities[0].cityName, "Bangalore");
    cities[0].numBuses = 1;
    strcpy(cities[0].buses[0].busNumber, "KA78EH1231");
    cities[0].buses[0].availableSeats = 5;
    strcpy(cities[1].buses[0].busType, "Non Sleeper A-C");

    strcpy(cities[1].cityName, "Hubli");
    cities[1].numBuses = 1;
    strcpy(cities[1].buses[0].busNumber, "KA32BZ3420");
    cities[1].buses[0].availableSeats = 4;
     strcpy(cities[1].buses[0].busType, "Non Sleeper A-C");

    strcpy(cities[2].cityName, "Mumbai");
    cities[2].numBuses = 1;
    strcpy(cities[2].buses[0].busNumber, "MH21OP9437");
    cities[2].buses[0].availableSeats = 5;
     strcpy(cities[2].buses[0].busType, "Non Sleeper A-C");

    strcpy(cities[3].cityName, "Kochi");
    cities[3].numBuses = 1;
    strcpy(cities[3].buses[0].busNumber, "KL50QW7128");
    cities[3].buses[0].availableSeats = 6;
     strcpy(cities[3].buses[0].busType, "Sleeper Non A-C");

    strcpy(cities[4].cityName, "Chennai");
    cities[4].numBuses = 1;
    strcpy(cities[4].buses[0].busNumber, "TN29XC1231");
    cities[4].buses[0].availableSeats = 4;
     strcpy(cities[4].buses[0].busType, "Non Sleeper Non A-C");

    strcpy(cities[5].cityName, "Madurai");
    cities[5].numBuses = 1;
    strcpy(cities[5].buses[0].busNumber, "TN65MY1231");
    cities[5].buses[0].availableSeats = 6;
     strcpy(cities[5].buses[0].busType, "Sleeper A-C");
    
    strcpy(cities[6].cityName, "Gandhinagar");
    cities[6].numBuses = 1;
    strcpy(cities[6].buses[0].busNumber, "GJ63FG2345");
    cities[6].buses[0].availableSeats = 5;
     strcpy(cities[6].buses[0].busType, "Sleeper A-C");
    
    strcpy(cities[7].cityName, "Jaipur");
    cities[7].numBuses = 1;
    strcpy(cities[7].buses[0].busNumber, "RJ48HI6789");
    cities[7].buses[0].availableSeats = 7;
     strcpy(cities[7].buses[0].busType, "Sleeper A-C");

     strcpy(cities[8].cityName, "Delhi");
    cities[8].numBuses = 1;
    strcpy(cities[8].buses[0].busNumber, "DL34JK9012");
    cities[8].buses[0].availableSeats = 5;
     strcpy(cities[8].buses[0].busType, "Sleeper A-C");

     strcpy(cities[9].cityName, "Lucknow");
    cities[9].numBuses = 1;
    strcpy(cities[9].buses[0].busNumber, "UP12MN3456");
    cities[9].buses[0].availableSeats = 5;
     strcpy(cities[9].buses[0].busType, "Sleeper A-C");

     strcpy(cities[10].cityName, "Patna");
    cities[10].numBuses = 1;
    strcpy(cities[10].buses[0].busNumber, "BR09OP7890");
    cities[10].buses[0].availableSeats = 6;
     strcpy(cities[10].buses[0].busType, "Sleeper A-C");

     strcpy(cities[11].cityName, "Kolkata");
    cities[11].numBuses = 1;
    strcpy(cities[11].buses[0].busNumber, "WB56QR2345");
    cities[11].buses[0].availableSeats = 5;
     strcpy(cities[11].buses[0].busType, "Sleeper A-C");

     strcpy(cities[12].cityName, "Bhubaneshwar");
    cities[12].numBuses = 1;
    strcpy(cities[12].buses[0].busNumber, "OD23ST6789");
    cities[12].buses[0].availableSeats = 5;
     strcpy(cities[6].buses[0].busType, "Sleeper A-C");

     strcpy(cities[13].cityName, "Vishakapatnam");
    cities[13].numBuses = 1;
    strcpy(cities[13].buses[0].busNumber, "AP78UV9012");
    cities[13].buses[0].availableSeats = 7;
     strcpy(cities[13].buses[0].busType, "Sleeper A-C");

     strcpy(cities[14].cityName, "Nagpur");
    cities[14].numBuses = 1;
    strcpy(cities[14].buses[0].busNumber, "MH45WX3456");
    cities[14].buses[0].availableSeats = 5;
     strcpy(cities[14].buses[0].busType, "Sleeper A-C");

     strcpy(cities[15].cityName, "Indore");
    cities[15].numBuses = 1;
    strcpy(cities[15].buses[0].busNumber, "MP67YZ6789");
    cities[15].buses[0].availableSeats = 4;
     strcpy(cities[15].buses[0].busType, "Sleeper A-C");

     strcpy(cities[16].cityName, "Hyderabad");
    cities[16].numBuses = 1;
    strcpy(cities[16].buses[0].busNumber, "TS89AB9012");
    cities[16].buses[0].availableSeats = 5;
     strcpy(cities[16].buses[0].busType, "Sleeper A-C");

     strcpy(cities[17].cityName, "Kota");
    cities[17].numBuses = 1;
    strcpy(cities[17].buses[0].busNumber, "RJ12CD3456");
    cities[17].buses[0].availableSeats = 6;
     strcpy(cities[17].buses[0].busType, "Sleeper A-C");

     strcpy(cities[18].cityName, "Kanpur");
    cities[18].numBuses = 1;
    strcpy(cities[18].buses[0].busNumber, "UP34EF6789");
    cities[18].buses[0].availableSeats = 5;
     strcpy(cities[18].buses[0].busType, "Sleeper A-C");

    addEdge(1, 2, 400);
    addEdge(1, 4, 500);
    addEdge(1, 5, 300);
    addEdge(4, 6, 300);
    addEdge(5, 6, 500);
    addEdge(5, 17, 600);
    addEdge(5, 14, 800);
    addEdge(2, 17, 500);
    addEdge(2, 3, 600);
    addEdge(3, 16, 600);
    addEdge(3, 7, 600);
    addEdge(7, 16, 400);
    addEdge(7, 8, 600);
    addEdge(8, 18, 200);
    addEdge(8, 9, 300);
    addEdge(9, 16, 800);
    addEdge(9, 10, 500);
    addEdge(10, 19, 100);
    addEdge(10, 11, 500);
    addEdge(11, 12, 600);
    addEdge(12, 13, 400);
    addEdge(12, 15, 1000);
    addEdge(13, 14, 400);
    addEdge(13, 15, 800);
    addEdge(14, 15, 800);
    addEdge(15, 17, 500);
    addEdge(15, 16, 400);
    addEdge(15, 19, 700);
    addEdge(16, 18, 300);
    addEdge(16, 17, 800);

   char sourceCity[20], destinationCity[20];

   printf("Available Cities:\n");

    int numColumns = 4; 
    int numRows = (V + numColumns - 1) / numColumns; 

    for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
        int index = i + j * numRows;
        if (index < V) {
            printf("%-20s", cityNames[index]); 
        }
    }
    printf("\n");
}
    int source, destination;

    do {
    printf("Enter source city: ");
    scanf("%s", sourceCity);

    source = findCityIndex(sourceCity);

    if (source == -1) {
        printf("Invalid source city name. Please enter a valid city name.\n");
    }
} while (source == -1);

do {
    printf("Enter destination city: ");
    scanf("%s", destinationCity);

    destination = findCityIndex(destinationCity);

    if (destination == -1) {
        printf("Invalid destination city name. Please enter a valid city name.\n");
    }
} while (destination == -1);
        dijkstra(source, destination);

       int numTickets;

do {
    printf("Enter the number of tickets you want to book: ");
    scanf("%d", &numTickets);

    if (numTickets <= 0) {
        printf("Invalid number of tickets. Please enter a positive Integer > 0.\n");
    }
} while (numTickets <= 0);

int path[V];
int index = 0;

for (int i = destination - 1; i != -1; i = parent[i]) {
    path[index++] = i;
}

displayAndSaveTickets(sourceCity, destinationCity, numTickets, cities[source - 1].buses[0].busNumber, numTickets, path, index);
displayAndConfirmBooking(source, destination, numTickets);
printf("Your ticket is saved to output.html");

    return 0;
}