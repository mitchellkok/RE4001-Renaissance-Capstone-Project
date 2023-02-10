import styles from '../fetch.module.css';

function Reading(props) {
    return (
        <div>
        <div className={styles.reading} onClick={() => props.parentFunction()}>
          <h5 className={styles.h5_custom}>{props.device}</h5>
          <h5> {props.reading1} {props.unit1} </h5>
          <h5> {props.reading2} {props.unit2}</h5>
        </div>
        </div>
    )
}

export default Reading;