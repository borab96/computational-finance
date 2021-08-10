from option_price import Option
import time

if __name__=="__main__":
    opt = Option(40, 0.3, 0.5)
    t1 = time.time()
    print(opt.call(0, 0, 50))
    t2 = time.time()
    print("Seconds since epoch =", round(t2-t1, 3))