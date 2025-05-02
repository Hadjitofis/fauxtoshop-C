#include <stdio.h>
#include <stdlib.h>
#define H 54

int main() {
    FILE* fp = stdin;//Αποθηκεύει σην fp τη φωτογραφία που έλαβε με ανακατεύθυνση
    //Η φωτογραφία που θα εξάγουμε με ανακατεύθηνση
    unsigned char imageHeader[H];
    size_t bytesRead;
    bytesRead = fread(imageHeader, sizeof(unsigned char), H, fp);//Διαβάζουμε το header της φωτογραφίας
    if (bytesRead < H) {
        return 1;
    }
    if (imageHeader[0] != 'B' || imageHeader[1] != 'M')
        return 1;
    int width = *(int*)&imageHeader[18];// Διαβάζω το πλάτος απο το header στο byte 18
    int height = *(int*)&imageHeader[22];//Διαβάζω το Μήκος απο το header στο byte 22
    int padding = (4 - (width * 3) % 4) % 4; //Δηλώνω το padding για τη φωτογραφία
    // Calculate the size of the 'Other Data'
    size_t otherDataSize = *((int*)&imageHeader[10]) - 54;

    // Read the 'Other Data'
    unsigned char* otherData = malloc(otherDataSize * sizeof(unsigned char));
    size_t result = fread(otherData, sizeof(unsigned char), otherDataSize, fp);

    if (result != otherDataSize) { // Handle the error
        return 1;
    }
    unsigned char** pic = malloc(height * sizeof(unsigned char*));// Εξασφαλίζω τις θέσεις που χρειάζομαι για τον δυσδιαστατο πινακα pic
    for (int i = 0; i < height; i++) {
        pic[i] = malloc((width * 3 + padding) * sizeof(unsigned char)); //Ενώνω τον πίνακα με τις θέσεις που δέσμευσα
    }
    if (pic == NULL) //Αν δεν καταφέρω να δεσμεύσω μνήμη
    {

        for (int i = 0; i < height; i++) {
            free(pic[i]);
        }
        free(pic);
        return 1;
    }
    bytesRead = 0;
    for (int i = 0; i < height; i++) //Διαβάζω την εικόνα σε bytes και τα μεταφέρω στον πίνακα pic
    {
        bytesRead = fread(pic[i], sizeof(unsigned char), (width * 3) + padding, fp);
    }
    size_t expectedBytes;
    expectedBytes = (width * 3) + padding;
    if (bytesRead != expectedBytes) {
        // Χειρισμός σφάλματος, δεν διαβάστηκαν όλα τα bytes
        return 1;
    }
    unsigned char** newPic = malloc(width * sizeof(unsigned char*)); //Εξασφαλίζω τις θέσεις που χρειάζεται ο νέος πίνακας newPic

    for (int i = 0; i < width; i++)
    {
        newPic[i] = malloc((height * 3) * sizeof(unsigned char)); // Ενώνω τις θέσεις του νέου πίνακα με τις θέσεις που δέσμευσα

        if (newPic[i] == NULL) //Αν δεν καταφέρω να δεσμεύσω μνήμη
        {
            // Ελευθερώνω την μνήμη 
            for (int j = 0; j < i; j++) {
                free(newPic[j]);
            }
            free(newPic);
            for (int j = 0; j < height; j++) {
                free(pic[j]);
            }
            free(pic);
            return 1;
        }
    }
    int pixelArrayOffset = *(int*)&imageHeader[10];
    if (pixelArrayOffset < 0)
        return 0;
    for (int j = 0; j < width; j++)//Μεταφέρω κάθε Πιξελ στην αναλογη θέση ώστε να γυρίσει η εικόνα 90 μοίρες δεξιόστροφα
    {
        for (int i = height - 1; i >= 0; i--)
        {
            //μεταφέρω κάθε χρώμα ξεχωριστά
            newPic[width - 1 - j][i * 3] = pic[i][j * 3];       // Κόκκινο
            newPic[width - 1 - j][i * 3 + 1] = pic[i][j * 3 + 1]; // Πράσινο
            newPic[width - 1 - j][i * 3 + 2] = pic[i][j * 3 + 2]; // Μπλε
        }
    }
    unsigned char newImageHeader[H]; //Φτιάχνω το header του νέου πίνακα
    for (int i = 0; i < H; i++) {
        newImageHeader[i] = imageHeader[i];
    }
    //Βάζω το ύψος της πρώτης εικόνας σαν πλάτος της αλλης και ανάποδα
    *(int*)&newImageHeader[18] = height;
    *(int*)&newImageHeader[22] = width;
    int newPadding = (4 - (height * 3) % 4) % 4; //  Δημιουργώ το padding της νέας εικόνας
    FILE* fo = stdout;

    fwrite(newImageHeader, sizeof(unsigned char), H, fo);// Βάζω στο αρχέιο που θα επιστρέψω την νέα ειικόνα απο τον πίνακα newPic
    // Write the 'Other Data' to the fo file
    fwrite(otherData, sizeof(unsigned char), otherDataSize, fo);
    for (int i = 0; i < width; i++) {
        fwrite(newPic[i], sizeof(unsigned char), (height * 3), fo);
        // Βάζω το padding για την νέα εικόνα
        for (int j = 0; j < newPadding; j++) {
            fputc(0, fo);
        }
    }
    //Απελευθερώνω την μνήμη
    for (int i = 0; i < height; i++) {
        free(pic[i]);
    }
    free(pic);
    free(otherData);
    return 0;
}
