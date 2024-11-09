import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

const FeatureList = [
  {
    title: 'Various problem types',
    description: (
      <>
        Supports batch and interactive problems, as well as ICPC- and IOI-style problems.
      </>
    ),
  },
  {
    title: 'I/O format validation',
    description: (
      <>
        Input/output formats can be validated declaratively.
      </>
    ),
  },
  {
    title: 'Local grading',
    description: (
      <>
        Solutions can be tested locally against generated test cases.
      </>
    ),
  },
];

function Feature({Svg, title, description}) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
