import numpy as np

difs = np.array([[150,-7.5], [150,  7.5]])
cart_d = 200

action_fields = 9
state_fields = 144
state_shift = np.array([0, 6, 36, 72])

posx = 0
posy = 0
posa = 0
mtrL = 0
mtrR = 0

print_file = 0

def reset():
    global posx
    global posy
    global posa
    global mtrL
    global mtrR

    posx = np.random.uniform(-10,10) if not(print_file) else np.random.uniform(-5,5)
    posy = np.random.uniform(-10,10) if not(print_file) else np.random.uniform(-5,5)
    posa = np.random.uniform(-0.17,0.17) if not(print_file) else np.random.uniform(-0.85,0.85)
    mtrL = 0
    mtrR = 0
    
    if print_file:
        print_file.write('%f\t%f' % (posx,posy))
        angs = np.array([np.sin(posa),np.cos(posa)])
        for i in range(2):
            print_file.write('\t%f' % sum((difs * angs)[i] + posy))
    
    return [mtrL, mtrR]

def step(action):
    global posx
    global posy
    global posa
    global mtrL
    global mtrR
    mL = np.clip(mtrL + action // 3 - 1, 0, 5)
    mR = np.clip(mtrR + action % 3 - 1, 0, 5)
    act_ml = np.random.uniform(0.45 * (mL + mtrL), 0.55 * (mL + mtrL))
    act_mr = np.random.uniform(0.45 * (mR + mtrR), 0.55 * (mR + mtrR))
    mid_angle = posa + (act_mr - act_ml) / (4 * cart_d)
    posx += np.cos(mid_angle) * (act_mr + act_ml) / 2
    posy += np.sin(mid_angle) * (act_mr + act_ml) / 2
    posa += (act_mr - act_ml) / (2 * cart_d)
    mtrL = mL
    mtrR = mR
    
    if print_file:
        print_file.write('\n%f\t%f' % (posx,posy))
        angs = np.array([np.sin(posa),np.cos(posa)])
        for i in range(2):
            print_file.write('\t%f' % sum((difs * angs)[i] + posy))
    
    return [mtrL, mtrR]

def get_sens():
    global posx
    global posy
    global posa
    global mtrL
    global mtrR
    sens = []
    angs = np.array([np.sin(posa),np.cos(posa)])
    for i in range(2):
        sens.append(1 if -10 < sum((difs * angs)[i] + posy) < 10 else 0)
    
    return sens

def check_done(sens):
    global posx
    global posy
    global posa
    global mtrL
    global mtrR
    if posx > 1000:
        done = 1
    elif not(np.any(sens)):
        done = -1
    else:
        done = 0
    
    return done

