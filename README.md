# MiniFactory


## L01: Conveyor Line 
 
```mermaid
sequenceDiagram
    participant M as main.py
    participant W as GymWrapper
    participant E as InventoryManagementEnv
    participant A as PPOAgent

    M->>W: train(num_episodes)
    W->>E: reset()
    E-->>W: initial state

    loop Episode Loop
        W->>A: select_action(state)
        A-->>W: action, log_prob, value
        W->>E: step(action)
        E-->>W: next_state, reward, done

        W->>A: store_transition(state, action, log_prob, reward, value, done)
        
        alt If Buffer Full
            W->>A: finish_trajectory()
        end
    end

    W->>M: Training Completed