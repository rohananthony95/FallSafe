var btn = document.querySelector('#review_button');

btn.addEventListener('click', randomize_review);

var rn_old = 0;

function randomize_review()
{
    var fieldNameElement = document.getElementById('review_text');
    
    var num = 13;
    
    var rn = Math.floor(num*Math.random())
    
    if (rn_old == rn)
        {
            rn = (rn + 1)%num;
        }
    
    if (rn == 0)
    {
        fieldNameElement.innerHTML = '"Go home Evanston Yelp, you\'re drunk. How this place has a 4-star rating is a greater mystery to me than the origins of the universe. I bought a sad webcam and my kids bought some even sadder (probably microwaved) gaming consoles. Sad to say it because this place seems like it has a ton of poitential as a great \'neighbourhood NSA\' but I won\'t be returning unless they get a liquor license." - Jeff B.';
    }
    
    else if (rn == 1)
    {
        fieldNameElement.innerHTML = '"5 STARS. A MASTAHPIECE." - Timothy Huang';
    }
    else if (rn == 2)
    {
        fieldNameElement.innerHTML = '"If I hadn\'t already invested in Blaze Pizza then I would definitely put $1,000,000,000 into these ambitious young men" - LeBron James';
    }
    else if (rn == 3)
    {
        fieldNameElement.innerHTML = '"Why did we do this 🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔🤔" - Jason Blacher';
    }
    else if (rn == 4)
    {
        fieldNameElement.innerHTML = '"Did somebody say my name?" - JR Smith, Greatest 3pt shooter alive     ';
        var JRimg = document.createElement("img");
        JRimg.src = "../webcam/images/JR.jpg";
        JRimg.width = 200;
        fieldNameElement.appendChild(JRimg);
    }
    else if (rn == 5)
    {
        fieldNameElement.innerHTML = '"I gave these guys $3 million on Shark Tank and all they made was this dumb webcam" - Mark Cuban';
    }
    else if (rn == 6)
    {
        fieldNameElement.innerHTML = '"These guys seem nice, but their product doesn\'t involve carrying any balls, so I\'ll have to pass on it." - Justin Jackson';
    }
    else if (rn == 7)
    {
        fieldNameElement.innerHTML = '"I knew from the moment Rami and Jason stepped foot in my class that they would build an incredible website and a functioning webcam, and boy was I right.  I can\'t wait to use their product to monitor all of the other EECS labs! If I could give them 6 stars, I would. - Ilya (probably)';
    }
    else if (rn == 8)
    {
        fieldNameElement.innerHTML = '"This product is great. The quality is top-notch.  As a budding online entertainer, it helps me reach so many more viewers.  Thanks to this webcam, I now have almost 10 friends on typeracer.com!  Come check out my page! ;)" - Michael Jiang';
    }
    else if (rn == 9)
    {
        fieldNameElement.innerHTML = '"This webcam is a wonderful product.  I have installed at least five units in nearly every building on campus.  Now, I can watch as all of my minions make me the most powerful man on earth........go cats!" - Morty Schapiro';
    }
    else if (rn == 10)
    {
        fieldNameElement.innerHTML = '*growl* *growl* *growl*........*growl* *raises paw* - Willie the Wildcat';
    }
    else if (rn == 11)
    {
        fieldNameElement.innerHTML = 'I love this webcam.  As a reluctant scientologist, I am constantly being recorded by my religious peers, and they use this webcam, so it must be good!" - Tom Cruise';
    }
    else if (rn == 12)
    {
        fieldNameElement.innerHTML = 'This webcam has chganged my life.  As a proud Wisconsinite, I use this webcam to watch my cheese mature, which saves me a whole trip out to the cellar every month.  I now use that time to socialize with friends.  Thanks, JRSmith Automation Systems!" - Jack Arnold';
    }
        
    rn_old = rn;
}