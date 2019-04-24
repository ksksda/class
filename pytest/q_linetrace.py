import numpy as np
import sim as sim
from sim import get_sens
from sim import check_done

def digitize_state(observation):
    return sum(sim.state_shift * np.array(observation))

def get_action(next_state, episode):
    epsilon = 0.5 * (1 / (episode + 1))
    if epsilon <= np.random.uniform(0, 1):
        next_action = np.argmax(q_table[next_state])
    else:
        next_action = np.random.choice(range(sim.action_fields))
        r = np.random.uniform(0, sum(np.array(q_table[next_state]) + 1))
        lim = 0
        for i in range(sim.action_fields):
            lim += q_table[next_state][i] + 1
            if r < lim:
                next_action = i
                break
    
    return next_action

def update_Qtable(q_table, state, action, reward, next_state):
    gamma = 0.99
    alpha = 0.5
    next_Max_Q=max(q_table[next_state])
    q_table[state, action] = (1 - alpha) * q_table[state, action] + alpha * (reward + gamma * next_Max_Q)
    return q_table

max_number_of_steps = 1000
num_consecutive_iterations = 20
num_episodes = 10000
goal_average_reward = 1.0
q_table = np.random.uniform(-1, 1, (sim.state_fields, sim.action_fields))

total_reward_vec = np.zeros(num_consecutive_iterations)

for episode in range(num_episodes):
    observation = sim.reset()
    sens = get_sens()
    observation.extend(sens)
    if check_done(sens):
        episode -= 1
        if sim.print_file:
            sim.print_file.close()
            sim.print_file = open('result.txt','w')
        continue
    state = digitize_state(observation)
    action = np.argmax(q_table[state])
    episode_reward = 0
    
    for t in range(max_number_of_steps):
        observation = sim.step(action)
        sens = get_sens()
        observation.extend(sens)
        done = check_done(sens)
        
        episode_reward += done
        
        next_state = digitize_state(observation)
        q_table = update_Qtable(q_table, state, action, done, next_state)
        
        action = get_action(next_state, episode)
        
        state = next_state
        
        if done:
            total_reward_vec = np.hstack((total_reward_vec[1:], episode_reward))
            print('%d Episode finished after %d time steps / mean %f' %
                  (episode, t + 1, total_reward_vec.mean()))
            break
    
    if sim.print_file:
        sim.print_file.close()
        if done > 0:
            break
        else:
            sim.print_file = open('result.txt','w')
    
    elif (total_reward_vec.mean() >= goal_average_reward):
        print('Episode %d train agent successfuly!' % episode)
        sim.print_file = open('result.txt','w')

