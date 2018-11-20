var w;
var columns;
var rows;
var board;
var squares;
var next;
var attackLevel = 1.0;
var releaseLevel = 0;
var block = false;
var attackTime = 0.01
var decayTime = 0.2;
var susPercent = 0.2;
var releaseTime = 0.6;

var env, triOsc;
env = new p5.Env();
env.setADSR(attackTime, decayTime, susPercent, releaseTime);
env.setRange(attackLevel, releaseLevel);

triOsc = new p5.Oscillator('triangle');
triOsc.amp(env);
triOsc.start();
var enabled = []
var previousMillis = 0;
var collisionsLookup = {
    "c1": { 'x': 0, 'y': 0, 'active': false }
}
var slider;
var previousXSliderValue = 0;
var previousXSliderValue2 = 0;
var previousYSliderValue1 = 0;
var previousYSliderValue2 = 0;
var w = 22;
var collision2Active = false;
var collision3Active = false;

function setup() {
    createCanvas(2000, 400);


    w = 22;
    // Calculate columns and rows
    columns = 48;
    rows = 18;
    // Wacky way to make a 2D array is JS
    board = new Array(columns);
    squares = new Array(columns);
    for (var i = 0; i < columns; i++) {
        board[i] = new Array(rows);
        squares[i] = new Array(rows);
    }
    // Going to use multiple 2D arrays and swap them
    next = new Array(columns);
    for (i = 0; i < columns; i++) {
        next[i] = new Array(rows);
    }
    init();

    for (var i = 0; i < columns; i++) {
        for (var j = 0; j < rows; j++) {
            squares[i][j] = new Square(i, j);
        }
    }
    slider = createSlider(0, 6, 0);
    slider.position(248, 430);

    xAxisSlider2 = createSlider(0, 6, 0);
    xAxisSlider2.position(400, 430);


    text("slider 2", 0, 30)
    yAxisSlider = createSlider(0, 10, 0);
    yAxisSlider.position(0, 20);

    yAxisSlider2 = createSlider(0, 10, 0);
    yAxisSlider2.position(0, 200);



    text("slider 1", 248, 440)

}


var collision1Active = false;

function draw() {
    var d = new Date();


    // print(yAxisSlider.value())
    background(255);
    // squares[5][17].enabled = true;
    // squares[5][17].level = 1;

    //   squares[10][10].enabled = true;
    // squares[10][10].level = 1;

    // Continually loop through all columns and rows
    // Check for state and either trigger squares on or turn them off
    for (var i = 0; i < columns; i++) {
        for (var j = 0; j < rows; j++) {

            if (squares[i][j].enabled) {

                if (squares[i][j].level == 1 && squares[i][j].enabled) {

                    fill(255, 204, 0);
                } else if (squares[i][j].level == 2 && squares[i][j].enabled) {

                    fill(255, 204, 255);
                } else if (squares[i][j].level == 3 && squares[i][j].enabled) {

                    fill(100, 100, 255);
                } else if (squares[i][j].level == 11 || squares[i][j].level == 22) {
                    // print(squares[i][j].level)
                    fill(20, 200, 30);
                }

            } else {
                fill(255);

            }

            stroke(0);
            rect(210 + (i * w), j * w, w - 1, w - 1);
        }
    }



    //Slider value is slider x axis on the bottom 
    sliderValue = slider.value() * 3;
    xsliderValue2 = xAxisSlider2.value() * 3;

    // code to draw out lines, only trigger this when you've seen a change in slider values
    // in order to work within the draw function and be able to 'erase lines' we are setting all the values in the line to false
    // then we are starting fro the beginning of the slider value and setting it to true 3 at a time 
    if (!collision1Active) {

        drawXSliderLine(5, sliderValue, previousXSliderValue)
      

    }
    previousXSliderValue = sliderValue;

    yAxisSliderValue = yAxisSlider.value() * 5;
    yAxisSliderValue2 = yAxisSlider2.value() * 5;


    // run the same calculations for the y side 
    if (!collision1Active) {
        // // Check if top y slider has moved
        drawSliderLine(5, yAxisSliderValue, previousYSliderValue1)

    }


    if (!collision2Active && !collision3Active) {
        //               // if either y axis slider is changed go through the process again 
        drawSliderLine(10, yAxisSliderValue2, previousYSliderValue2)

    }


    if (!collision3Active) {
        //               // if either y axis slider is changed go through the process again 
        // print("drawing collision 3")
            drawXSliderLine(10, xsliderValue2, previousXSliderValue2)
    }

    previousYSliderValue1 = yAxisSliderValue;
    previousYSliderValue2 = yAxisSliderValue2;



}


function drawXSliderLine(xIndex, xSliderValue, previousXSlideValue) {


    if (previousXSlideValue != xSliderValue) {
        // loop through the rows and disable on column 5
        for (var j = 0; j < 17; j++) {
            //Checking if x is enabled so that we don't turn off an overlapping square
            if (!squares[xIndex][17 - j].xEnabled) {
                squares[xIndex][j].level = 3;
                squares[xIndex][j].enabled = false;
                board[xIndex][j] = 0;
                // print(slider.value())
                squares[xIndex][17 - j].yEnabled = false;

            }
        }

        // enable squares on column 5 how high the slider is raised
        for (var j = 0; j < xSliderValue; j++) {
            squares[xIndex][17 - j].level = 11;
            squares[xIndex][17 - j].enabled = true;
            squares[xIndex][17 - j].yEnabled = true;
        }
    }
}


function drawSliderLine(yIndex, ySliderValue, previousYSlideValue) {
    if (previousYSlideValue != ySliderValue) {
        print('IN herenow')
        for (var j = 0; j < 48; j++) {

            // if (!squares[47 - j][10].xEnabled) {

            squares[47 - j][yIndex].level = 0;
            squares[47 - j][yIndex].enabled = false;
            squares[47 - j][yIndex].xEnabled = false;

        }
    }


    for (var j = 0; j < ySliderValue - 2; j++) {
        // print("This is happening")
        squares[j][yIndex].level = 22;
        squares[j][yIndex].enabled = true;
        squares[j][yIndex].xEnabled = true;


    }


}

function clearColor() {
    for (var i = 0; i < columns; i++) {
        for (var j = 0; j < rows; j++) {

            fill(255);

            stroke(0);
            rect(i * w, j * w, w - 1, w - 1);
        }
    }



}
// reset board when mouse is pressed
function mouseReleased() {
    // print('squares in beginning')
    // print(squares)

    sliderValue = slider.value() * 3;

        // Check if collision is over
    if (collision3Active) {
        print("Turning off collision 3")

        if (xAxisSlider2.value() < 5) {
            print("Turning off collision 2 via x")
            collision3Active = false;
            
            switchMatrixState(10, 10, 0);
        }

        if (yAxisSlider2.value() < 2) {
            collision3Active = false;
            
            print("here")
            switchMatrixState(10, 10, 0);
            // squares[5][10].level = 22;
            // squares[5][10].enabled = true;
        }

    }

    // Check if collision is over
    if (collision2Active) {
        print("Turning off collision 2")

        if (slider.value() < 5) {
            print("Turning off collision 2 via x")
            collision1Active = false;
            collision2Active = false;
            switchMatrixState(5, 10, 0);
        }

        if (yAxisSlider2.value() < 2) {
            collision1Active = false;
            collision2Active = false;
            print("here")
            switchMatrixState(5, 10, 0);
            // squares[5][10].level = 22;
            // squares[5][10].enabled = true;
        }

    }

    if (collision1Active) {

        // print("slider value" + slider.value())
        //Update this to not be hard coded
        print("Turning off collision 1")
        if (slider.value() < 5) {
            print("Turning off collision 1 via x")
            collision1Active = false;
            collision2Active = false;
            switchMatrixState(5, 5, 0);
        }

        if (yAxisSlider.value() < 2) {
            collision1Active = false;
            collision2Active = false;
            switchMatrixState(5, 5, 0);
            // squares[5][5].level = 22;
            // squares[5][5].enabled = true;
        }


    }


    //this is the code that turns on collisions 
    if (!collision1Active) {

        createCollision(5,5)
    
    }

    //check for collisions on 2
    if (!collision2Active) {
        createCollision(5,10)

    }

     if (!collision3Active) {
        createCollision(10,10)

    }



    print('finAL squares')
    print(squares)

}

var drawYIndex = 0;

function createCollision(xColumnIndex,yColumnIndex) {

    for (var j = 0; j < 18; j++) {

        if (squares[j][yColumnIndex].xEnabled && squares[j][yColumnIndex].yEnabled && j== xColumnIndex) {
            print("IN HERE in collision  with j  " + j)
            print("y column" + yColumnIndex)
                        print("x column" + xColumnIndex )



            // Figure out how to set this programatically
            if (yColumnIndex == 10 && xColumnIndex ==5 ) {
                print('111')
                collision2Active = true;
                drawYIndex = 14
                drawXIndex = 46
            }

            if (yColumnIndex == 5  && xColumnIndex ==5) {
                print("2222")
                collision1Active = true;
                drawYIndex = 9
                drawXIndex = 46
            }

            if (xColumnIndex == 10 && yColumnIndex == 10) {
                print('3333')
                collision3Active = true;
                drawYIndex = 14
                drawXIndex = 41
            }

            // collision2Active = true;
            //turn everything off so you can setup a collision
            for (var i = 0; i < drawXIndex; i++) {
                squares[47 - i][yColumnIndex].level = 0;
                squares[47 - i][yColumnIndex].enabled = false;
                squares[47 - i][yColumnIndex].yEnabled = false;

            }

            // turn off all default lights 'above the collision'
            // var drawIndex = 17 - j -4v
            // var drawIndex = 14
            // if(yColumnIndex == 5) {
            //         drawIndex = 9

            // }

            for (var i = 0; i < drawYIndex; i++) {
                print("starting draw from : " )
                print(17 - j -2)
                squares[xColumnIndex][i].level = 0;
                squares[xColumnIndex][i].enabled = false;
                squares[xColumnIndex][i].yEnabled = false;


            }
            switchMatrixState(j, yColumnIndex, 1);
            // print(squares)

        }
    }


}


function switchMatrixState(xCoordinate, yCoordinate, state) {


    if (state == 0) {
        print("switchMatrixState off")
        print("origin is  x : " + xCoordinate + " y : " + yCoordinate)

        fillSquare(xCoordinate, yCoordinate, 0, 1);
        fillSquare(xCoordinate, yCoordinate, 0, 2);
        fillSquare(xCoordinate, yCoordinate, 0, 3);

    } else {

        // triOsc.freq(220);
        // env.play(triOsc, 0, 0.2);


        fillSquare(xCoordinate, yCoordinate, 1, 1);
        print("done filling");
        // setTimeout(function() {

        fillSquare(xCoordinate, yCoordinate, 1, 2)
        triOsc.freq(440);
        print("done filling2 ");
        // env.play(triOsc, 0, 0.2);


        // }, 1000)
        // setTimeout(function() {
        fillSquare(xCoordinate, yCoordinate, 1, 3);
        triOsc.freq(660);
        // env.play(triOsc, 0, 0.2);
        print("done filling3");


        // }, 2000);

        // setTimeout(function() {
        //      clearColor();

        // }, 1000);



    }

}


function fillSquare(xCoordinate, yCoordinate, state, level) {
    var offset = 0;
    if (level == 1) {

        offset = 3;

    } else if (level == 2) {
        offset = 5;

    } else if (level == 3) {

        offset = 7;
    }

    print("origin is  x : " + xCoordinate + " y : " + yCoordinate)


    for (x = 0; x < offset; x++) {
        xindex = xCoordinate - level + x;
        for (i = 0; i < offset; i++) {
            yindex = yCoordinate - level + i;
            // print("yCoordinate + " + yCoordinate)
            // print("x index : " + xindex + ' y index : ' + yindex)
            if (state == 1 && !squares[xindex][yindex].enabled) {
                squares[xindex][yindex].enabled = true;
                squares[xindex][yindex].level = level;
                //         stroke(0);
                //            fill(100, 200, 255);
                // rect(210 + (xindex * w), yindex * w, w - 1, w - 1);

            }


            if (state == 0) {
                print("Switching off with offset" + offset)
                // print("switching off x : " + xindex + " y : " + yindex)
                squares[xindex][yindex].enabled = false;
                squares[xindex][yindex].xEnabled = false;
                squares[xindex][yindex].yynabled = false;
                squares[xindex][yindex].level = 0;

            }

            // board[xindex][yindex] = state;
        }
    }
}

// Fill board randomly
function init() {

}