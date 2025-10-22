#include "machstate.h"

//remove this var's

bool flag;
int counter; //contador para ver se todos colocaram a smallbet
int smallbet=20;    //aposta obrigatória em cada jogada


void MachState::updMach(){
    switch (currentState) {
    case init:

        if ( 3<= table->player_list.length() && table->player_list.length() <=8 ){
            currentState = ante;
        }

        break;

    case ante:

        if (counter == table->player_list.length()){
            prevstate = ante; //antes de sair define o estado anterior como Hcards
            currentState = Hcards;
        }

        break;

    case Hcards:

        flag = true;

        foreach (Players_t *pl, table->player_list) {
            if( pl->hand.size() != 2 )
            {
                flag = false; // Flag state  = false, when a player doesn't have two cards
            }
        }

        if(flag == true)
        {
            prevstate = Hcards; //antes de sair define o estado anterior como Hcards
            currentState = bet;
        }

        break;


    case bet:

        flag = true;

        foreach (Players_t *pl, table->player_list) {
            if( pl->loopstatus != "done" || pl->loopstatus != "imout" )
            {
                flag = false;
            }
        }

        if(this->smwp->WcurrentState == 8 && flag == true){

            if ( winplayer->status == "winner" || prevstate == river ){
                //travar esta ME até a maquina updBet estiverpronta
                currentState = winner;
            }

            if ( prevstate == Hcards ){
                //travar esta ME até a maquina updBet estiverpronta
                currentState = flop;
            }

            if ( prevstate == flop ){
                //travar esta ME até a maquina updBet estiverpronta
                currentState = turn;
            }

            if ( prevstate == turn ){
                //travar esta ME até a maquina updBet estiverpronta
                currentState = river;
            }
        }

        break;

    case flop:
        //mostrar as 3 cartas na mesa           table->drawcards(3)
        if(table->cards_on_table.length()==3){
            prevstate = flop;
            currentState = bet;
        }

        break;

    case turn:
        //mostrar mais 1 carta na mesa          table->drawcards(1)
        if(table->cards_on_table.length()==4){
            prevstate = turn;
            currentState = river;
        }

        break;

    case river:
        //mostrar a ultima carta da mesa        table->drawcards(1)
        if(table->cards_on_table.length()==5){
            prevstate = river;
            currentState = bet;
        }

        break;


    case winner:
        if ( winplayer->status == "winner" ){
            prevstate = winner;
            currentState = reset;
        }
        else{
            //winplayer = winninghand();  //isto se a winninghand retornar o jogador que vencer a mão
            prevstate = winner;
            currentState = reset;
        }

        break;

    case reset:
        //wplayer->chips += tablechips;
        //table->total_chips = 0;
        //recolher todas as cartas. tanto da mesa como dos jogadores
        //voltar a colocar os jogadores que tinham saido da jogada em jogo
        if (3<= table->player_list.length() && table->player_list.length() <=8){
            prevstate = reset;
            currentState = ante;
        }

        break;

    default:
        break;
    }
}

Bet::Bet(table_t *t, Whoplay *wp)
{
    this->table = t;
    this->smwp = wp;
}

void Bet::updBet(){
    switch (BcurrentState){

    case Waiting:
        if ( this->smwp->WcurrentState > 7 ){
            if (table->player_list[this->smwp->WcurrentState]->status == "myturn"){
                BcurrentState = Decision;
            }
        }

        break;

    case Decision:
        if (table->player_list[this->smwp->WcurrentState]->status == "Fold"){
            BcurrentState = Fold;
        }

        if (table->player_list[this->smwp->WcurrentState]->status == "Call"){
            BcurrentState = Call;
        }

        if (table->player_list[this->smwp->WcurrentState]->status == "Check"){
            BcurrentState = Check;
        }

        if (table->player_list[this->smwp->WcurrentState]->status == "Raise"){
            BcurrentState = Raise;
        }

        if (table->player_list[this->smwp->WcurrentState]->status == "Allin"){
            BcurrentState = Allin;
        }

        break;

    case Fold:
        //player leaves that game:
        //inplayers.splice(lenght(inplayers), 1))
        //inNplayers--;
        if(table->player_list[this->smwp->WcurrentState]->loopstatus=="imout"){
            BcurrentState = Waiting;
        }

        break;

    case Call:
        //playerchips[0]-=rvalue;
        //tablechips+=rvalue;
        if(table->player_list[this->smwp->WcurrentState]->loopstatus=="done"){
            BcurrentState = Waiting;
        }

        break;

    case Check:
        if(table->player_list[this->smwp->WcurrentState]->loopstatus=="done"){
            BcurrentState = Waiting;
        }

        break;

    case Raise:
        if(table->player_list[this->smwp->WcurrentState]->loopstatus=="iraised"){
            BcurrentState = Waiting;
        }

        break;
        /*
            if (rvalue==0){     //ve se é um raise por cima de outro
                //ask for rvalue
                //playsleft = inNplayers-1;   //cada 1 joga mais 1 vez
                //playerchips[currplayer]-=rvalue;
                //tablechips+=rvalue;
                BcurrentState = Waiting;
                break;
            }else{
                rvalue+=newrvalue;
                //ask for newvalue
                playsleft = inNplayers-1;   //cada 1 joga mais 1 vez
                playerchips[currplayer]-=newrvalue;
                tablechips+=newrvalue;
                BcurrentState = Waiting;
                break;
            }*/

    case Allin:
        if((table->player_list[this->smwp->WcurrentState]->loopstatus=="iraised") || (table->player_list[this->smwp->WcurrentState]->loopstatus=="done")){
            BcurrentState = Waiting;
        }

        break;
        /*
            if(rvalue!=0 && playerchips[currplayer]>rvalue){        //correspondente ao Raise do raise
                newrvalue = playerchips[currplayer];
                tablechips += newrvalue;
                playerchips[currplayer] = 0;
                BcurrentState = Waiting;
                break;
            }
            if(rvalue==0){                                          //correspondente ao RAISE
                rvalue = playerchips[currplayer];
                tablechips += rvalue;
                playerchips[currplayer] = 0;
                playsleft = inNplayers-1;   //cada 1 joga mais 1 vez
                BcurrentState = Waiting;
                break;
            }
            if(rvalue!=0 && playerchips[currplayer]<=rvalue){        //correspondente ao CALL
                tablechips += playerchips[currplayer];
                playerchips[currplayer] = 0;
                BcurrentState = Waiting;
                break;
            }*/

    default:
        break;
    }
}

Whoplay::Whoplay(table_t *t)
{
    this->table = t;
}

void Whoplay::updwhoplays()
{
    switch(WcurrentState){

    case Wait:
        if( round == true )
            WcurrentState=Player1;
        break;

    case Player1:

        if( table->player_list[Player1]->loopstatus == "done" || table->player_list[Player1]->loopstatus == "imout" ){
            int i = WcurrentState;
            i++;
            WcurrentState = static_cast<Whoplays>(i);
            if (fp){
                table->lastminbetidx=0;
                fp = false;
            }
        }

        break;

    case Player2:

        if( table->player_list[Player2]->loopstatus == "done" || table->player_list[Player2]->loopstatus == "imout" ){

            /*if( WcurrentState == (table->player_list.length()-1)){
                WcurrentState=Player1;
                foreach(Client_t *pl, table->player_list){
                    pl->loopstatus = "";
                }
            } else{
                WcurrentState=Player3;
            }*/
            int i = WcurrentState;
            i++;
            qDebug() << i;
            if(i >= table->player_list.length()){
                i = 0;
            }
            while(table->player_list[i]->loopstatus == "imout"){
                i++;
                if(i >= (table->player_list.length() - 1)){
                    i = 0;
                }
            }
            WcurrentState = static_cast<Whoplays>(i);
        }

        break;

    case Player3:

        if( table->player_list[Player3]->loopstatus == "done" || table->player_list[Player3]->loopstatus == "imout" ){

            /*if( WcurrentState == (table->player_list.length()-1)){
                WcurrentState=Player1;
                foreach(Client_t *pl, table->player_list){
                    pl->loopstatus = "";
                }
            } else{
                WcurrentState=Player4;
            }*/

            int i = WcurrentState;
            i++;
            if(i >= table->player_list.length()){
                i = 0;
            }
            while(table->player_list[i]->loopstatus == "imout"){
                i++;
                if(i >= (table->player_list.length() - 1)){
                    i = 0;
                }
            }
            WcurrentState = static_cast<Whoplays>(i);
        }

        break;

    case Player4:

        if( table->player_list[Player4]->loopstatus == "done" || table->player_list[Player4]->loopstatus == "imout" ){

            /*if( WcurrentState == (table->player_list.length()-1)){
                WcurrentState=Player1;
                foreach(Client_t *pl, table->player_list){
                    pl->loopstatus = "";
                }
            } else{
                WcurrentState=Player5;
            }*/

            int i = WcurrentState;
            i++;
            if(i >= table->player_list.length()){
                i = 0;
            }
            while(table->player_list[i]->loopstatus == "imout"){
                i++;
                if(i >= (table->player_list.length() - 1)){
                    i = 0;
                }
            }
            WcurrentState = static_cast<Whoplays>(i);
        }

        break;

    case Player5:

        if( table->player_list[Player5]->loopstatus == "done" || table->player_list[Player5]->loopstatus == "imout" ){

            /*if( WcurrentState == (table->player_list.length()-1)){
                WcurrentState=Player1;
                foreach(Client_t *pl, table->player_list){
                    pl->loopstatus = "";
                }
            } else{
                WcurrentState=Player6;
            }*/

            int i = WcurrentState;
            i++;
            if(i >= table->player_list.length()){
                i = 0;
            }
            while(table->player_list[i]->loopstatus == "imout"){
                i++;
                if(i >= (table->player_list.length() - 1)){
                    i = 0;
                }
            }
            WcurrentState = static_cast<Whoplays>(i);
        }

        break;

    case Player6:

        if( table->player_list[Player6]->loopstatus == "done" || table->player_list[Player6]->loopstatus == "imout" ){

            /*if( WcurrentState == (table->player_list.length()-1)){
                WcurrentState=Player1;
                foreach(Client_t *pl, table->player_list){
                    pl->loopstatus = "";
                }
            } else{
                WcurrentState=Player7;
            }*/

            int i = WcurrentState;
            i++;
            if(i >= table->player_list.length()){
                i = 0;
            }
            while(table->player_list[i]->loopstatus == "imout"){
                i++;
                if(i >= (table->player_list.length() - 1)){
                    i = 0;
                }
            }
            WcurrentState = static_cast<Whoplays>(i);
        }

        break;

    case Player7:

        if( table->player_list[Player7]->loopstatus == "done" || table->player_list[Player7]->loopstatus == "imout" ){

            /*if( WcurrentState == (table->player_list.length()-1)){
                WcurrentState=Player1;
                foreach(Client_t *pl, table->player_list){
                    pl->loopstatus = "";
                }
            } else{
                WcurrentState=Player8;
            }*/

            int i = WcurrentState;
            i++;
            if(i >= table->player_list.length()){
                i = 0;
            }
            while(table->player_list[i]->loopstatus == "imout"){
                i++;
                if(i >= (table->player_list.length() - 1)){
                    i = 0;
                }
            }
            WcurrentState = static_cast<Whoplays>(i);
        }

        break;

    case Player8:

        if( table->player_list[Player8]->loopstatus == "done" || table->player_list[Player8]->loopstatus == "imout" ){
            /*WcurrentState=Player1;
            foreach(Client_t *pl, table->player_list){
                pl->loopstatus = "";
            }*/

            int i = WcurrentState;
            i++;
            if(i >= table->player_list.length()){
                i = 0;
            }
            while(table->player_list[i]->loopstatus == "imout"){
                i++;
                if(i >= (table->player_list.length() - 1)){
                    i = 0;
                }
            }
            WcurrentState = static_cast<Whoplays>(i);
        }

        break;
    }

}


