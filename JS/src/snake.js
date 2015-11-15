var graph = null;
var graph_width = parseInt($(window).width() / 10) * 10;
var graph_height = parseInt($(window).height() / 30) * 20;
$("#graph").css('width', graph_width + "px");
$("#graph").css('height', graph_height + "px");
$("#graph").css('margin', "0 auto");
var min_size = 10;

var snake = null;
var snake_speed = 100;
var square_num = 0;
var dir = 0;
var food_x = 0;
var food_y = 0;

var timerMove = null;

function initGrid()
{
    $("#graph").html("");
	graph = d3.select('#graph')
			.append('svg')
			.attr('width',graph_width)
			.attr('height',graph_height);
	for (var x = 0; x <= graph_width; x+=min_size)
	{
		graph.append('line')
			.attr('x1',x)
			.attr('y1',0)
			.attr('x2',x)
			.attr('y2',0 + graph_height)
			.attr('style','stroke:rgb(199,199,199);stroke-width:1');	 				
	}
	for (var y = 0; y <= graph_height; y+=min_size)
	{
		graph.append('line')
			.attr('x1',0)
			.attr('y1',y)
			.attr('x2',0 + graph_width)
			.attr('y2',y)
			.attr('style','stroke:rgb(199,199,199);stroke-width:1');	 				
	}
}

function initSnake()
{
	snake = new Array(2);
	snake[0] = new Array();
	snake[1] = new Array();
	//snake[0][0] = parseInt(Math.random() * graph_width   / min_size) * min_size;
	//snake[1][0] = parseInt(Math.random() * graph_height  / min_size) * min_size;
	snake[0][0] = graph_width / 2;
	snake[1][0] = graph_height / 2;
	
	square_num = square_num + 1;;
}

function existInSnake(food_x, food_y)
{
	for (var i = 0; i < square_num; i++)
	{
		if (snake[0][i] == x && snake[1][i] == y)
		{
			return true;
		}
	}
	return false;
}

function addSnakeFood()
{
	do 
	{
		x = parseInt(Math.random() * graph_width   / min_size) * min_size;
		y = parseInt(Math.random() * graph_height  / min_size) * min_size;
	}while(existInSnake(food_x, food_y));		
	food_x = x;
	food_y = y;
}

function checkEating()
{
	switch(dir)
	{
		case 0:
			if (snake[0][0] == food_x && snake[1][0] - 10 == food_y)
			{
				return true;
			}
			break;
		case 1:
			if (snake[0][0] + 10 == food_x && snake[1][0] == food_y)
			{
				return true;
			}
			break;
		case 2: 
			if (snake[0][0] == food_x && snake[1][0] + 10 == food_y)
			{
				return true;
			}
			break;
		case 3:
			if (snake[0][0] - 10 == food_x && snake[1][0] == food_y)
			{
				return true;
			}
			break;
	}
	return false;
}

function moveSnake()
{	
	graph.selectAll('rect').remove();
	if (snake[0][0] < 0 || snake[0][0] >= graph_width || snake[1][0] < 0 || snake[1][0] >= graph_height)
	{
		clearInterval(timerMove);
		timerMove = null;
		alert("恭喜您, 你一共吃了 " + (square_num - 1) + " 个食物.");
		document.title = "我用贪吃蛇吃了" + (square_num - 1) + "个食物";
		square_num = 0;
		return;
	}
	if (checkEating())
	{
		for (var i = square_num; i >= 1; i--)
		{
			snake[0][i] = snake[0][i-1];
			snake[1][i] = snake[1][i-1];
		}
		snake[0][0] = food_x;
		snake[1][0] = food_y;
		square_num = square_num + 1;
		addSnakeFood();
	}
	else
	{
		for (var i = square_num - 1; i > 0; i--)
		{
			snake[0][i] = snake[0][i-1];
			snake[1][i] = snake[1][i-1];
		}
		//0: up; 1: right; 2: down; 3: left;
		switch(dir)
		{
			case 0:
				snake[1][0] = snake[1][0] - 10;
				break;
			case 1:
				snake[0][0] = snake[0][0] + 10;
				break;
			case 2: 
				snake[1][0] = snake[1][0] + 10;
				break;
			case 3:
				snake[0][0] = snake[0][0] - 10;
				break;
		}
	}
	paintSnake();
}

function paintSnake()
{
	graph.append('rect')
			.attr('width', min_size)
			.attr('height', min_size)
			.attr('x', snake[0][0])
			.attr('y', snake[1][0])
			.attr('style', 'fill:rgb(0,0,255);opacity:0.8');

	for (var i = 1; i < square_num; i++)
	{
		graph.append('rect')
			.attr('width', min_size)
			.attr('height', min_size)
			.attr('x', snake[0][i])
			.attr('y', snake[1][i])
			.attr('style', 'fill:rgb(199,199,199);opacity:0.8');
	}
	
	graph.append('rect')
		.attr('width', min_size)
		.attr('height', min_size)
		.attr('x', food_x)
		.attr('y', food_y)
		.attr('style', 'fill:rgb(255,0,0);opacity:0.8');	
}

function startSnake()
{
	if (null == timerMove)
	{
		timerMove = setInterval(moveSnake, snake_speed);		
	}
	if (0 == square_num)
	{
		initSnake();
		addSnakeFood();
	}
}

function pauseSnake()
{
	if (null != timerMove)
	{
		clearInterval(timerMove);		
		timerMove = null;
	}
}	

$(window).load(
	 function()
	 {
		initGrid();
		initSnake();
		addSnakeFood();
		
		timerMove = setInterval(moveSnake, snake_speed);
		
		$('body').on('keydown',function (event)
		{
			switch(event.which)
			{
				case "w".charCodeAt(0):
				case "W".charCodeAt(0):
				case 38:
					dir = 0;
					break;
				case "d".charCodeAt(0):	
				case "D".charCodeAt(0):
				case 39:								
					dir = 1;
					break;
				case "s".charCodeAt(0):
				case "S".charCodeAt(0):
				case 40:	
					dir = 2;
					break;
				case "a".charCodeAt(0):
				case "A".charCodeAt(0):
				case 37:				
					dir = 3;
					break;
			}
		});
		$("body").swipe( {
			swipe:function(event, direction, distance, duration, fingerCount) {
				switch(direction)
				{
					case "up":
						dir = 0;
						break;
					case "right":
						dir = 1;
						break;
					case "down":
						dir = 2;
						break;
					case "left":
						dir = 3;
						break;
				}
			},
		});		
	  }
 );
