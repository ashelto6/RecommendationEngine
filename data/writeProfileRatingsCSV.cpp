#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main()
{
    std::vector<std::string> names = {"Tony","Rodger","Charles","David", "Tom", "Jason", "Darlene", 
    "Herbert", "Danika","Jamie","Yoselin","Gunner","Meadow","Jaylen","Octavio","Esmeralda","Natalya","Abram","Shyla","Leila","Liberty",
    "Brogan","Emiliano","Angel","Donavan","Milton","Amy","Rayne","Elyse","Wesley","Allyson","Winston","Amani","Hector","Eliana","Alexia",
    "Tyson","Christina","Douglas","Ramiro","Jose","Kaylee","Christian","Justice","Imani","Carla","Jaylin","Shaun","Milo","Cora","Desmond",
    "Danica","Ramon","Noel","Talon","Alani","Sullivan","Helen","Tianna","Landen","Anna","Olivia","Ada","Jefferson","Demarion","Allan",
    "Jesse","Maria","Rhianna","Russell","Todd","Ariel","Autumn","Jennifer","Evie","Jaydan","Alice","Ariella","Jadon","Moriah","Leah",
    "Augustus","Marlee","Kennedy","Quentin","Mara","Dorian","Heidi","Leroy","Leonardo","Arnav","Deborah","Reynaldo","Janae","Kirsten",
    "Vaughn","Mollie","Lisa","Denise","Makenna","Chase","Lucian","Darius","Andrew","Kobe","Lilyana","Anabella","Aniya","Olive","Jairo",
    "Aspen","Jeffrey","Sarahi","Addison","Zack","Tiffany","Kelvin","Benjamin","Janiyah","Jaslene","Clarence","Kolby","Thomas","Josue",
    "Amber","Jaylan","Kyan","Larissa","Yandel","Jerry","Judith","Edwin","Frankie","Miracle","Harper","Annika","Kiara","Lorena","Mathias",
    "Camryn","Lilliana","Jermaine","Axel","Adrien","Allen","Eve","Karma","Max","Emmalee","Atticus","Hailey","Lilian","Karson","Seth",
    "Kenneth","Evangeline","Shamar","Monserrat","Janiya","Quinten","Perla","Madison","Ayden","Cohen","Marisol","Corinne","Frederick",
    "Sanaa","Aiyana","Braiden","Sean","Kaylah","Owen","Annie","Dennis","Lillian","Lincoln","Marvin","Kaiden","Finley","Lara","Joy",
    "Daniela","Ellis","Maverick","Stephen","Mike","Cameron","Malachi","Haley","Paige","Genesis","Michaela","Davian","Tamia","Nikolai",
    "Daniel","Amaris","Luca","Austin","Madden","Waylon","Casey","Zion","Ryleigh","Lyric","Moises","Marcos","Sidney","Sophia","Charlie",
    "Lilia","Annalise","Davion","Marc","Weston","Keaton","Kellen","Brycen","Will","Calvin","Charlize","Raven","Micheal","Jayden","Thalia",
    "Ana","Fernando","Jackson","Isabel","Samara","Ezequiel","Osvaldo","Jenna","Abby","Amara","Fabian","Adriel","Salma","Amya","Tristin",
    "Jade","Josiah","Haven","Sierra","Matias","Kyleigh","Lukas","Giselle","Rishi","Kason","Dania","Rodolfo","Darwin","Nancy","Chana",
    "Louis","Ryann","Carmelo","Wayne","Yadiel","Edith","Erika","Harmony","Sofia","Ernesto","Lexi","Brayan","Lance","Itzel","Carleigh",
    "Marie","Kathy","Miriam","Joey","Angelica","Ruben","Valentina","Theodore","Evelyn","Krish","Ibrahim","Julio","Peyton","Brendon","Lana",
    "Jaxon","Amira","Halle","Haylie","Gregory","Mia","Heath","Mario","Trey","Emery","Trace","Angela","Cody","Laura","Briana","Victor",
    "Jazmine","Diana","Marcus","Raul","Tanner","Celia","Nolan","Ava","Maximo","Jimena","Frank","Alicia","Barbara","Ean","Lewis","Ignacio",
    "Nikhil","Angel","Tomas","Savanna","Leslie","Jacoby","Josephine","Theresa","Simone","Jovanni","Brisa","Melody","Paxton","Alfonso",
    "Josh","Susan","Yaritza","Haleigh","Jadyn","Dayami","Trevor","Cassidy","Jamie","Amanda","Agustin","Lea","Bo","Alexa","Abbey","Alex","Cordell",
    "Pierre","Seamus","Keira","Jabari","Madeline","Cesar","Ivy","Chance","Camila","Craig","Micah","Lawrence","Adelaide","Tori","Paris","Jax","Uriel",
    "Valeria","Isabell","Noelle","Yaretzi","Korbin","Lia","Jayden","Luke","Sanai","Kiera","Dylan","Zechariah","Hanna","Sydnee","Magdalena",
    "Jane","Brynn","Madelynn","Jamar","Breanna","Roselyn","Coleman","Zaria","Mohamed","Eli","Ali","Donald","Erica","Isabella","Brooks","Van",
    "Quinn","Beau","Rene","Jayvion","Jamiya","Ciara","Karlie","Maximilian","Hudson","Aaliyah","Mohammed","Hugo","Adelyn","Maren","Fernanda",
    "Parker","Mikaela","Ayla","Bailey","Johnathan","Elliott","Ezra","Raymond","Case","Ryland","Luciano","Anika","Kieran","Tobias","Essence",
    "Ismael","Donna","Matthias","Taniyah","Shelby","Elizabeth","Caden","Ethen","Ashley","Arely","Antony","Giuliana","Celeste","Hayden","Mckayla",
    "Sheldon","Whitney","Rocco","Gabe","Marquis","Briley","Gaige","Clinton","Kale","Mohammad","Emilee","Joe","Audrina","Natalie","Rosemary","Noah","Melissa",
    "Yusuf","Priscilla","Chaim","Teresa","Giovanni","Anthony","Devin","Justice","Irene","Kaitlyn","Jovany","Jeramiah","Koen","Jaida","Dahlia",
    "Emery","Hunter","Kaylynn","Asa","Delilah","Camren","Brianna","Jonas","Neil","Keegan","Andrea","Logan","Ryker","Lawson","Jaeden","Billy",
    "Kailyn","Griffin","Ximena","Ashlynn","Maleah","Makayla","Neveah","Cara","Lennon","Aldo"};

    srand(time(nullptr));
    std::ofstream myfile;
    myfile.open ("profileRatings.csv");
    for(auto& name : names)
    {
        myfile << name;
        for(int i = 0; i < 98791; ++i)
        {
            int num = rand() % 3 + -1; 
            myfile << "," << num;
        }
        myfile << "\n";
    }
    myfile.close();
}