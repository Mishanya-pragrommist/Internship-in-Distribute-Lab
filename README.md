This is the task about order book. Here I had to create an order book with the base asset as UAH and quote asset as USD ($)
So how to use this programm? Here is how:
 1) Enter your ID (can be any number excepto for 0 and less), side: 0 - to buy UAH or 1 - to sell UAH, amount of bought/sold currencies and price for each UAH
 2) Then you will see your order. There will be shown all entered info + total price for order (amount of currencies * price). Also you will see sorted lists of selling and buying orders
    (they are generated randomly). Your order will be in one of the lists
 3) If there is at least one match, you will find that order in "Finding matches" section. If you are sure you want to trade, press y/Y, otherwise n/N. After successful trade you can see
    balance changes for you and another trader.

For this task I used 2 classes:
 - class Order: contains user ID, amount of currencies, price and total price (in general, all info that user enters). As methods there are getters and overloaded operator '<<' for easy outputing
 - class OrderBook contains vectors selling_people and buying_people, where all orders stores. Also there are methods sort_selling_people() and sort_buying_people() for sorting orders (in selling order - from     
   lowest to highest, in buying - vice versa). For outputing there is overloaded '<<', just like in Order class, that outputs all orders into console window.
To input data from user I created function user_input(), that not only reads data but also checks its correctness (all this data cannot be <= 0, side can be only 0 or 1). If some of entered info is incorrect,
you will get a message about this. Otherwise, one object of class Order will be created with entered data. Then object of class OrderBook will be created and filled with randomly generated orders. Also your
order will be pushed into one of the lists (if side = 0 -> into buying_people, if side = 1 -> into selling_people).
To find a match I created function find_best_match() that goes through all elements of opposite list to one where your order is and finds the best match:
 * if you are buying, function will find order in the selling_people vector that has the least price or equal to your price
 * if you are selling, func will find order in the buying_people vector that has biggest price or equal to your one

If match was found, you can accept trade by pressing 'y' or cancel it by 'n'. If you accepted, function show_balance_changes() will show how many money you have spent/bought on the order. If there are no
matches, you will see a message about this.

Why is that solution effitient?
 - it doesn't allow user entering incorrect data.
 - all orders is represented by a quite simple and convenient class which makes creating every order such an easy process.
 - all orders stores in the vectors of order book which means you can easily add as much orders as you want and you can get very easy access to each of them.
 - all orders are always sorted (when you add one to the order book, they sorts automatically).
 - if progarmm found a match and you accepted it, you can know how much did you spent money for purchasing some currencies or bought from a trade 
