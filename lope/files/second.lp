#*
    SAMPLE FILE
*#

#* INPUT *#
kuha ("%d", &grade);        #Input Statement

#* OUTPUT *#

lahad("Ilagay ang grado:"); #Output Statement with linya
lahad(i);                   #Output with identifier;
lahad("Marka: %d", 87/2);   #Output with linya and expression


#* DECLARATION *#

puntonumero midterm_grade;      #Declaration with identifier
doblenumero raw_grade = grade;  #Declared Assignment with identifier

#* ASSIGNMENT *#

midterm_grade = 1.16;       #Assignment by value
raw_grade += midterm_grade; #Assignment with variable
raw_grade = midterm_grade + final_grade / 2;
                            #Assignment with expression

#* CONDITIONAL *#


kung (grade) {
    lahad ("Pagbati! Ikaw ay pumasa!"); #Kung
}

kung (grade >= 75) {
    lahad ("Pagbati! Ikaw ay pumasa!"); #Kung Kundi
}kundi{
    lahad ("Bagsak ikaw!");
}

kung (grade >= 90) {
    lahad("Magaling! Ikaw ay pumasa!");
}sakali(grade >= 75){
    lahad("Pagbati! Ikaw ay pumasa");   #Kung Sakali Kundi
}kundi{
    lahad ("Bagsak ikaw!");
}

kung (grade >=65)
{
    kung (grade >= 90) {
        lahad("Marka: A");
    }sakali (grade >= 80) {
        lahad("Marka: B");
    }sakali (grade >= 70) {
        lahad("Marka: C");               #Nested Kundi
    }sakali (grade >= 60) {
        lahad("Marka: D");
    }kundi {
        lahad("Marka: E");
    }
}kundi {
    lahad("Marka: F");
}

#* ITERATIVE *#

para (num j = 0; i<3; i++) {    #Para
    lahad(i);
}

habang (i != 0) {               #Habang
    lahad(i);
}

para (i = 0; i<3; i+=1) {            #Para
    para (num j = 0; j<3; j+=1) {    #Nested Para
        lahad(i*j);
    }
}
habang (i != 0) {                   #Habang
    habang (j != 0) {               #Nested Habang
        lahad(i+j);
        j--;
    }i--;
}
