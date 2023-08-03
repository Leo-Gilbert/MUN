from engi1020.arduino import *
import random
import time
from time import sleep

analogportrotarydial = 3
analogportemperature = 0
digitalportnobuzzer = 4

def startTimer():
  global start
  start = time.time()

def endTimer():
  return (time.time() - start)

def roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer):
    gamenum = random.randint(1,4)
    if gamenum == 1: ##Buzzer minigame
        print("Listen to the buzzer! ")
        buzznum = random.randint(1,10)
        buzzcounter = 0
        print(buzznum)
        while buzzcounter < buzznum:
            buzzer_note(digitalportnobuzzer, 400, 100)
            sleep(0.5)
            buzzcounter += 1
        return buzznum
    elif gamenum == 2: ##Temperature mini game
        randomtemp = round(random.uniform(25.00, 28.00), 2)
        print("Find the temperature! ")
        print(randomtemp)
        while True:
            realtemp = round(temp_celsius(analogportemperature), 2)
            if realtemp >= randomtemp:
                buzzer_note(digitalportnobuzzer, 400, 1000)
                print("Temperature found! ")
                lcd_print(f"{randomtemp}")
                sleep(2)
                lcd_clear()
                break
        return randomtemp
    elif gamenum == 3: ##screen colour mini game
        print("What colour is the screen? ")
        colval = 0
        randomcolour = round(random.uniform(0.001, 1), 3)
        print(randomcolour)
        if 0 < randomcolour < 0.060:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("RED")
            colval = "RED"
        elif 0.060 <= randomcolour < 0.100:
            lcd_hsv(randomcolour,1 , 255)
            lcd_print("ORANGE")
            colval = "ORANGE"
        elif 0.100 <= randomcolour < 0.210:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("YELLOW")
            colval = "YELLOW"
        elif 0.210 <= randomcolour < 0.350:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("GREEN")
            colval = "GREEN"
        elif 0.350 <= randomcolour < 0.450:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("TILE")
            colval = "TILE"
        elif 0.450 <= randomcolour < 0.664:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("BLUE")
            colval = "BLUE"
        elif 0.664 <= randomcolour < 0.747:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("PURPLE")
            colval = "PURPLE"
        elif 0.747 <= randomcolour < 0.830:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("MAJENTA")
            colval = "MAJENTA"
        elif 0.830 <= randomcolour < 0.913:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("PINK")
            colval = "PINk"
        elif 0.913 <= randomcolour <= 1:
            lcd_hsv(randomcolour, 1, 255)
            lcd_print("RED")
            colval = "RED"
        sleep(2)
        lcd_clear()
        return colval
    elif gamenum == 4: ##servo mini game
        print("Find the rotary dial position! ")
        randompos = random.randint(0,1023)
        analogpos = analog_read(int(analogportrotarydial))
        while analogpos != randompos:
            analogpos = analog_read(int(analogportrotarydial))
            print(randompos)
            print(analogpos)
            if analogpos < randompos:
                lcd_print("Too low! ")
                sleep(2)
                lcd_clear()
                sleep(2)
            elif analogpos > randompos:
                lcd_print("Too high! ")
                sleep(2)
                lcd_clear()
                sleep(2)
            if (analogpos - 5) < randompos < (analogpos + 5):
                sleep(2)
                lcd_print(f"Position: {randompos}")
                sleep(2)
                lcd_clear()
                break
        return randompos

def checkInput(memoryvalues, roundnumber):
    memoryvalues = list(memoryvalues)
    counter = 1
    print(f"Values for round {roundnumber}")
    for j in memoryvalues:
        userinput = input(f"Enter value #{counter}: ")
        counter += 1
        if userinput.isalpha() == True:
                userinput = userinput.upper()
        if userinput != j:
            print("Incorrect! Better luck next time. ")
            exit()

def memoryGame(analogportrotarydial, analogportemperature, digitalportnobuzzer):
    username = str(input("Welcome! Please enter your name: "))
    startTimer()
    print("Round 1 starting!")
    round1vals = []
    round1mg1 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round1vals.append(round1mg1)
    checkInput(round1vals, 1)
    print("Round 2 starting! ")
    round2vals = []
    round2mg1 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round2vals.append(round2mg1)
    round2mg2 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round2vals.append(round2mg2)
    checkInput(round1vals, 1)
    checkInput(round2vals, 2)
    print("Round 3 starting! ")
    round3vals = []
    round3mg1 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round3vals.append(round3mg1)
    round3mg2 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round3vals.append(round3mg2)
    round3mg3 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round3vals.append(round3mg3)
    checkInput(round1vals, 1)
    checkInput(round2vals, 2)
    checkInput(round3vals, 3)
    print("Round 4 starting!")
    round4vals = []
    round4mg1 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round4vals.append(round4mg1)
    round4mg2 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round4vals.append(round4mg2)
    round4mg3 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round4vals.append(round4mg3)
    round4mg4 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round4vals.append(round4mg4)
    checkInput(round1vals, 1)
    checkInput(round2vals, 2)
    checkInput(round3vals, 3)
    checkInput(round4vals, 4)
    print("Round 5 starting! Final Round! ")
    round5vals = []
    round5mg1 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round5vals.append(round5mg1)
    round5mg2 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round5vals.append(round5mg2)
    round5mg3 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round5vals.append(round5mg3)
    round5mg4 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round5vals.append(round5mg4)
    round5mg5 = str(roundMinigame(analogportrotarydial, analogportemperature, digitalportnobuzzer))
    round5vals.append(round5mg5)
    checkInput(round1vals, 1)
    checkInput(round2vals, 2)
    checkInput(round3vals, 3)
    checkInput(round4vals, 4)
    checkInput(round5vals, 5)
    t = endTimer()
    print(f"Game complete! Well Done {username}. Your time: {t}")

memoryGame(3,0,4)